/*****************************************************
 *  Implementation of "BasicBlock" and "FlowGraph".
 *
 */

#include "tac/flow_graph.hpp"
#include "3rdparty/map.hpp"
#include "config.hpp"
#include "tac/tac.hpp"
#include <unordered_map>

using namespace mind;
using namespace mind::tac;
using namespace mind::util;

// temp variable comparator
struct temp_less {
    bool operator()(Temp t1, Temp t2) { return (t1->id < t2->id); }
};

/* Auxilliary function for printing variable sets.
 *
 * PARAMETERS:
 *   os    - the output stream
 *   s     - the variable set
 * RETURNS:
 *   the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Set<Temp> *s) {
    os << "[";

    if (NULL != s && !s->empty()) {
        size_t sz = s->size();
        Temp *buf = new Temp[sz];
        std::copy(s->begin(), s->end(), buf);
        std::sort(buf, buf + sz, temp_less());
        os << buf[0];
        for (size_t i = 1; i < sz; ++i)
            os << " " << buf[i];
    }
    os << "]";

    return os;
}

/****************************** for BasicBlock
 * ************************************/

/* Constructor.
 */
BasicBlock::BasicBlock(void) {
    bb_num = -1;
    tac_chain = NULL;
    in_degree = 0;
    end_kind = BY_JUMP;
    var = NULL;
    next[0] = next[1] = -1;
    cancelled = false;

    Def = new Set<Temp>();     // empty set
    LiveUse = new Set<Temp>(); // empty set
    LiveIn = new Set<Temp>();  // empty set
    LiveOut = new Set<Temp>(); // empty set
}

/* Prints the basic block.
 *
 * PARAMETERS:
 *   os    - the output stream
 */
void BasicBlock::dump(std::ostream &os) {
    os << "===============================================" << std::endl;
    // prints the basic information
    os << "* BASICK BLOCK " << bb_num << ": " << std::endl;
    os << "*   Def     = " << Def << std::endl;
    os << "*   LiveUse = " << LiveUse << std::endl;
    os << "*   LiveIn  = " << LiveIn << std::endl;
    os << "*   LiveOut = " << LiveOut << std::endl;

    os << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;

    // prints the tacs
    for (Tac *t = tac_chain; t != NULL; t = t->next)
        os << "| " << t;

    os << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;

    // prints the end kind
    switch (end_kind) {
    case BY_JUMP:
        os << "*   END BY JUMP, goto " << next[0] << std::endl;
        break;

    case BY_JZERO:
        os << "*   END BY JZERO, if " << var << " = " << std::endl;
        os << "*      0: goto " << next[0] << "; 1: goto " << next[1]
           << std::endl;
        break;

    case BY_RETURN:
        os << "*   END BY RETURN, result = " << var << std::endl;
        break;
    }
    os << "===============================================" << std::endl;
}

/****************************** for FlowGraph
 * ************************************/

/* Deletes all the Memo TAC's (internal used).
 *
 * PARAMETERS:
 *   f     - the Functy object to be cleaned
 */
static void deleteMemo(Functy f) {
    while (NULL != f->code && f->code->op_code == Tac::MEMO)
        f->code = f->code->next;

    for (Tac *t = f->code; NULL != t; t = t->next) {
        if (Tac::MEMO == t->op_code) {
            t->prev->next = t->next;
            if (NULL != t->next)
                t->next->prev = t->prev;
        }
    }
}

/* Marks which TAC belongs to which block (the 1st pass of building CFG).
 *
 * PARAMETERS:
 *   t     - a TAC sequence
 * RETURNS:
 *   how many basic blocks have been identified
 */
static int markBasicBlocks(Tac *t) {
    int index = -1;
    bool at_start = false;

    for (; t != NULL; t = t->next) {
        t->bb_num = index;

        // detects ending
        switch (t->op_code) {
        case Tac::RETURN:
        case Tac::JUMP:
        case Tac::JZERO:
            index++; // terminates a basic block
            at_start = true;
            break;

        // detects beginning
        case Tac::MARK:
            if (!t->op0.label->target) {
                // not target label: deletes it
                t->prev->next = t->next; // we know t->prev cannot be NULL
                if (NULL != t->next)
                    t->next->prev = t->prev;

            } else {
                // target label: starts a new basic block
                if (!at_start) {
                    ++index;
                    t->bb_num = index;
                    at_start = true; // it is already in a new block!
                }
            }
            break;

        default:
            at_start = false;
            break;
        }
    }

    // return the number of identified basic blocks
    return index;
}

/* Gathers the marked basic blocks (the 2nd pass of building CFG).
 *
 * PARAMETERS:
 *   start - head of the marked TAC sequence
 *   bbv   - the basic block vector (a container for the gathered blocks)
 */
static void gatherBasicBlocks(Tac *start, Vector<BasicBlock *> &bbv) {
    BasicBlock *current = NULL;
    Tac *next_start = NULL;
    Tac *end = NULL;

    // skips the preambling tacs
    while (NULL != start && start->bb_num < 0)
        start = start->next;

    for (; start != NULL; start = next_start) {
        // locates the actual start node
        while (Tac::MARK == start->op_code)
            start = start->next;
        start->prev = NULL;

        // locates the end node
        end = start;
        while ((NULL != end->next) && (end->next->bb_num == start->bb_num))
            end = end->next;
        next_start = end->next;

        // builds basic block
        current = new BasicBlock();
        current->bb_num = start->bb_num;
        current->tac_chain = start;

        // determines end kind
        switch (end->op_code) {
        case Tac::RETURN:
            current->end_kind = BasicBlock::BY_RETURN;
            current->var = end->op0.var;
            end = end->prev;
            break;

        case Tac::JUMP:
            mind_assert(NULL != end->op0.label->where);

            current->end_kind = BasicBlock::BY_JUMP;
            current->next[0] = current->next[1] = end->op0.label->where->bb_num;
            end = end->prev;
            break;

        case Tac::JZERO:
            mind_assert(NULL != next_start);

            current->end_kind = BasicBlock::BY_JZERO;
            current->var = end->op1.var;
            current->next[0] = end->op0.label->where->bb_num;
            current->next[1] = next_start->bb_num;
            end = end->prev;
            break;

        default:
            mind_assert(NULL != next_start);

            current->end_kind = BasicBlock::BY_JUMP;
            current->next[0] = current->next[1] = next_start->bb_num;
            break;
        }

        // adjusts the end pointer
        if (NULL == end)
            current->tac_chain = NULL;
        else
            end->next = NULL;

        // adds this block to the graph
        //	bbv.push_back(current);
        bbv[current->bb_num] = current;
    }
}

/* Builds a control-flow graph from a Functy object.
 *
 * PARAMETERS:
 *   f     - the functy object
 * RETURNS:
 *   the control-flow graph (un-optimized)
 */
FlowGraph *FlowGraph::makeGraph(Functy f) {
    mind_assert(NULL != f);

    FlowGraph *g = NULL;

    deleteMemo(f);

    g = new FlowGraph();
    g->_n = markBasicBlocks(f->code);
    g->_bbs.resize(g->_n);

    gatherBasicBlocks(f->code, g->_bbs);

    return g;
}

/* Simplifies (optimizes) a control-flow graph.
 *
 * NOTE:
 *   the optimizations include:
 *   1. eliminates empty END-BY-JUMP blocks
 *   2. reduces END-BY-JZERO blocks into END-BY-JUMP blocks
 *   3. eliminates all unreachable blocks
 *   the above steps are performed only once.
 */
void FlowGraph::simplify(void) {
    BasicBlock *b = NULL;
    BasicBlock *trace = NULL;

    // computes in degrees
    _bbs[0]->in_degree = 1; // entrance has an in degree of at least 1

    for (int i = 0; i < _n; ++i) {
        switch (_bbs[i]->end_kind) {
        case BasicBlock::BY_JZERO:
            ++_bbs[_bbs[i]->next[1]]->in_degree;
            // falls through

        case BasicBlock::BY_JUMP:
            ++_bbs[_bbs[i]->next[0]]->in_degree;
            break;

        default:
            break;
        }
    }

    // cancels all unreachable blocks and all empty END-BY-JUMP blocks
    for (int i = 0; i < _n; ++i) {
        b = _bbs[i];
        if (b->in_degree <= 0 ||
            (b->end_kind == BasicBlock::BY_JUMP && NULL == b->tac_chain)) {
            b->cancelled = true;
        }
    }

    // tries to optimize all traces
    for (int i = 0; i < _n; ++i) {
        b = _bbs[i];

        if (b->cancelled || b->end_kind == BasicBlock::BY_RETURN)
            continue;

        // forwards all the empty jumps
        trace = _bbs[b->next[0]];
        while (trace->cancelled)
            trace = _bbs[trace->next[0]]; // "trace" must be a BY_JUMP block
                                          // (why? :-)
        b->next[0] = trace->bb_num;

        if (b->end_kind == BasicBlock::BY_JZERO) {
            trace = _bbs[b->next[1]];
            while (trace->cancelled)
                trace = _bbs[trace->next[0]];
            b->next[1] = trace->bb_num;

            if (b->next[0] == b->next[1]) {
                b->end_kind =
                    BasicBlock::BY_JUMP; // reduce END-BY-JZERO into END-BY-JUMP
            }
        } else
            b->next[1] = b->next[0];
    }

    // this is a demo, so we don't do the above steps iteratively

    // shrinks the flow graph (and adjusts the block numbers)
    std::unordered_map<int, int> new_num; // old bb_num -> new bb_num
    int sz = 0;                           // new size

    for (int i = 0; i < _n; ++i) {
        if (!_bbs[i]->cancelled) {
            new_num[i] = sz;
            if (i > sz)
                _bbs[sz] = _bbs[i];
            ++sz;
        }
    }

    _n = sz;
    _bbs.resize(_n);

    for (int i = 0; i < _n; ++i) {
        _bbs[i]->bb_num = new_num[_bbs[i]->bb_num];

        if (BasicBlock::BY_RETURN != _bbs[i]->end_kind) {
            _bbs[i]->next[0] = new_num[_bbs[i]->next[0]];
            _bbs[i]->next[1] = new_num[_bbs[i]->next[1]];
        }
    }
}

/* Gets a specified basic block.
 *
 * PARAMETERS:
 *   i     - basic block number
 * RETURNS:
 *   the basic block identified by that number
 */
BasicBlock *FlowGraph::getBlock(int i) {
    mind_assert(i >= 0 && i < _n);

    return _bbs[i];
}

/* Gets the size of this CFG.
 *
 * RETURNS:
 *   the size (how many basic blocks) of this CFG
 */
size_t FlowGraph::size(void) { return _n; }

/* Gets a begin iterator.
 *
 * RETURNS:
 *   the begin iterator (pointing to the first block)
 */
FlowGraph::iterator FlowGraph::begin(void) { return _bbs.begin(); }

/* Gets an end iterator.
 *
 * RETURNS:
 *   the end iterator (pointing AFTER the last block)
 */
FlowGraph::iterator FlowGraph::end(void) { return _bbs.end(); }

/* Gets a begin reverse-iterator.
 *
 * RETURNS:
 *   the begin reverse-iterator (pointing to the last block)
 */
FlowGraph::reverse_iterator FlowGraph::rbegin(void) { return _bbs.rbegin(); }

/* Gets an end reverse-iterator.
 *
 * RETURNS:
 *   the end reverse-iterator (pointing BEFORE the first block)
 */
FlowGraph::reverse_iterator FlowGraph::rend(void) { return _bbs.rend(); }

/* Prints this CFG.
 *
 * PARAMETERS:
 *   os    - the output stream
 */
void FlowGraph::dump(std::ostream &os) {
    for (int i = 0; i < _n; i++) {
        _bbs[i]->dump(os);
        os << std::endl;
    }
}
