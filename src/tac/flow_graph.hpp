/*****************************************************
 *  Definition of "BasicBlock" and "FlowGraph".
 *  The flow graph and dataflow analysis part are almost finished,
 *  if you want to learn dataflow analysis with codes, please refer to codes here.
 * 
 *  Of course, if you add some new Tacs, 
 *  you are supposed to update the codes at line 38 and line 176 in tac/dataflow.cpp
 */

#ifndef __MIND_FLOWGRAPH__
#define __MIND_FLOWGRAPH__

#include "3rdparty/set.hpp"
#include "3rdparty/vector.hpp"
#include "asm/mach_desc.hpp"
#include "define.hpp"

#include <iostream>

namespace mind {

#define MIND_FLOWGRAPH_DEFINED
namespace tac {

/**
 * Basic Block.
 *
 * A basic block is a maximum piece of code that
 * contains no jump-out's (excluding function calls).
 */
struct BasicBlock {
    int bb_num; // basic block number

    enum {
        BY_JUMP,
        BY_JZERO,
        BY_RETURN
    } end_kind; // what kind of statement terminates this block

    int in_degree; // in degree in the control-flow graph

    Temp var; // for END-BY-JZERO blocks, it is the condition variable;
              // for END-BY-RETURN blocks, it is the return value.

    int next[2]; // the block number of the successors
                 // for END-BY-JZERO blocks, next[0] is the successor
                 //  of condition = 0, while next[1] is the successor
                 //  of condition = 1;
                 // for END-BY-JUMP blocks, next[0]=next[1]=successor

    bool cancelled; // internal flag for FlowGraph
    int mark;       // internal flag for MachDesc

    Tac *tac_chain; // the associated TAC sequence fragment

    assembly::Instr *instr_chain; // for ASM code generation: the associated assembly code sequence
    const char *entry_label; // for ASM code generation: the associated entry label in assembly code

    util::Set<Temp> *Def; // the DEF set: ALL variables defined in this block
    util::Set<Temp> *LiveUse; // the LiveUSE set: all used-before-defined variables
    util::Set<Temp> *LiveIn; // the LiveIn set: all variables alive at the entry
    util::Set<Temp> *LiveOut; // the LiveOut set: all variables alive at the exit

    // constructor
    BasicBlock();
    // computes the DEF set and LiveUse set
    void computeDefAndLiveUse(void); // in tac/dataflow.cpp
    // computes the LiveOut set of every TAC inside (see also:
    // FlowGraph::analyzeLiveness)
    void analyzeLiveness(void); // in tac/dataflow.cpp
    // prints this basic block
    void dump(std::ostream &);

    void updateLU(Temp);
    void updateDEF(Temp);
};

/**
 * Control-flow Graph (CFG).
 *
 * Control-flow graph is a set of basic blocks
 * (edges are the ``successor'' relationship).
 */
class FlowGraph {
  private:
    util::Vector<BasicBlock *> _bbs; // basic blocks
    int _n;                          // number of basic blocks

    FlowGraph() { /* don't invoke me */
    }

  public:
    typedef util::Vector<BasicBlock *>::iterator iterator;
    typedef util::Vector<BasicBlock *>::reverse_iterator reverse_iterator;

    // builds a control-flow graph from a Functy
    static FlowGraph *makeGraph(Functy);
    // simplifies (optimizes) a control-flow graph
    void simplify(void);
    // gets the specified basic block
    BasicBlock *getBlock(int);
    // gets the size of this control-flow graph
    size_t size(void);
    // gets the begin iterator (pointing to the first block)
    iterator begin(void);
    // gets the end iterator (pointing beyond the last block)
    iterator end(void);
    // gets the begin reverse iterator (pointing to the last block)
    reverse_iterator rbegin(void);
    // gets the end reverse iterator (pointing beyond the first block)
    reverse_iterator rend(void);
    // computes the LiveIn set and the LiveOut set of every basic block
    void analyzeLiveness(void); // in tac/dataflow.cpp
    // prints this graph
    void dump(std::ostream &);
};

} // namespace tac

// an auxilliary function for printing variable set
std::ostream &operator<<(std::ostream &, util::Set<tac::Temp> *);
} // namespace mind

#endif // __MIND_FLOWGRAPH__
