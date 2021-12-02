/*****************************************************
 *  Operations of Tac's and data objects.
 *
 *  Keltin Leung 
 */

#include "tac/tac.hpp"
#include "config.hpp"
#include "options.hpp"
#include "tac/flow_graph.hpp"

#include <iomanip>
#include <sstream>

using namespace mind;
using namespace mind::tac;

// a syntatic sugar for safety check
#define REQUIRE_I4(x) mind_assert(4 == x->size);

/* Allocating a new Tac object.
 *
 * PARAMETERS:
 *   code  - kind of the TAC
 * RETURNS:
 *   the newly allocated Tac object
 */
static Tac *allocateNewTac(Tac::Kind code) {
    Tac *t = new Tac;
    t->op_code = code;
    t->op0.ival = t->op1.ival = t->op2.ival = 0;
    t->bb_num = 0;
    t->mark = 0;
    t->prev = t->next = NULL;
    t->LiveOut = NULL;

    return t;
}

/* Creates a Memo tac.
 *
 * NOTE:
 *   memorandum, serving as comments
 * PARAMETERS:
 *   memo - content
 * RETURNS:
 *   a Memo tac
 */
Tac *Tac::Memo(const char *memo) {
    Tac *t = allocateNewTac(Tac::MEMO);
    t->op0.memo = memo;

    return t;
}

/* Creates an Add tac.
 *
 * NOTE:
 *   addition
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   an Add tac
 */
Tac *Tac::Add(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::ADD);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Sub tac.
 *
 * NOTE:
 *   subtraction
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1 (left)
 *   op2  - operand 2 (right)
 * RETURNS:
 *   a Sub tac
 */
Tac *Tac::Sub(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::SUB);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Mul tac.
 *
 * NOTE:
 *   multiplication
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a Mul tac
 */
Tac *Tac::Mul(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::MUL);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Div tac.
 *
 * NOTE:
 *   division
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1 (left)
 *   op2  - operand 2 (right)
 * RETURNS:
 *   a Div tac
 */
Tac *Tac::Div(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::DIV);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Mod tac.
 *
 * NOTE:
 *   modulo
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1 (left)
 *   op2  - operand 2 (right)
 * RETURNS:
 *   a Mod tac
 */
Tac *Tac::Mod(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::MOD);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates an Equ tac.
 *
 * NOTE:
 *   equality
 *   dest : 1 if (op1 == op2); 0 if (op1 != op2)
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   an Equ tac
 * NOTE:
 *   if you want "string" support in Mind, you should add
 *   a new runtime library function for comparision.
 */
Tac *Tac::Equ(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::EQU);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Neq tac.
 *
 * NOTE:
 *   inequality
 *   dest: 1 if (op1 != op2); 0 if (op1 == op2)
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a Neq tac
 */
Tac *Tac::Neq(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::NEQ);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Les tac.
 *
 * NOTE:
 *   less than
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a Les tac
 */
Tac *Tac::Les(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::LES);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Leq tac.
 *
 * NOTE:
 *   less than or equal to
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a Leq tac
 */
Tac *Tac::Leq(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::LEQ);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Gtr tac.
 *
 * NOTE:
 *   greater than
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a Gtr tac
 */
Tac *Tac::Gtr(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::GTR);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a Geq tac.
 *
 * NOTE:
 *   greater than or equal to
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a Geq tac
 */
Tac *Tac::Geq(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::GEQ);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a LAnd tac.
 *
 * NOTE:
 *   logical and
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a LAnd tac
 */
Tac *Tac::LAnd(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::LAND);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates a LOr tac.
 *
 * NOTE:
 *   logical or
 * PARAMETERS:
 *   dest - result
 *   op1  - operand 1
 *   op2  - operand 2
 * RETURNS:
 *   a LOr tac
 */
Tac *Tac::LOr(Temp dest, Temp op1, Temp op2) {
    REQUIRE_I4(dest);
    REQUIRE_I4(op1);
    REQUIRE_I4(op2);

    Tac *t = allocateNewTac(Tac::LOR);
    t->op0.var = dest;
    t->op1.var = op1;
    t->op2.var = op2;

    return t;
}

/* Creates an Assign tac.
 *
 * NOTE:
 *   assignment
 * PARAMETERS:
 *   dest - result
 *   src  - source
 * RETURNS:
 *   an Assign tac
 */
Tac *Tac::Assign(Temp dest, Temp src) {
    REQUIRE_I4(dest);
    REQUIRE_I4(src);

    Tac *t = allocateNewTac(Tac::ASSIGN);
    t->op0.var = dest;
    t->op1.var = src;

    return t;
}

/* Creates a Neg tac.
 *
 * NOTE:
 *   negation
 * PARAMETERS:
 *   dest - result
 *   src  - source
 * RETURNS:
 *   a Neg tac
 */
Tac *Tac::Neg(Temp dest, Temp src) {
    REQUIRE_I4(dest);
    REQUIRE_I4(src);

    Tac *t = allocateNewTac(Tac::NEG);
    t->op0.var = dest;
    t->op1.var = src;

    return t;
}

/* Creates a LNot tac.
 *
 * NOTE:
 *   logical not
 * PARAMETERS:
 *   dest - result
 *   src  - source
 * RETURNS:
 *   a LNot tac
 */
Tac *Tac::LNot(Temp dest, Temp src) {
    REQUIRE_I4(dest);
    REQUIRE_I4(src);

    Tac *t = allocateNewTac(Tac::LNOT);
    t->op0.var = dest;
    t->op1.var = src;

    return t;
}

/* Creates a BNot tac.
 *
 * NOTE:
 *   bitwise not
 * PARAMETERS:
 *   dest - result
 *   src  - source
 * RETURNS:
 *   a BNot tac
 */
Tac *Tac::BNot(Temp dest, Temp src) {
    REQUIRE_I4(dest);
    REQUIRE_I4(src);

    Tac *t = allocateNewTac(Tac::BNOT);
    t->op0.var = dest;
    t->op1.var = src;

    return t;
}

/* Creates a LoadImm4 tac.
 *
 * NOTE:
 *   load int32 immediate number
 * PARAMETERS:
 *   dest  - result
 *   value - the immediate number (32bit)
 * RETURNS:
 *   a LoadImm4 tac
 */
Tac *Tac::LoadImm4(Temp dest, int value) {
    REQUIRE_I4(dest);

    Tac *t = allocateNewTac(Tac::LOAD_IMM4);
    t->op0.var = dest;
    t->op1.ival = value;

    return t;
}

Tac *Tac::LoadSymbol(Temp dest, std::string label) {
    REQUIRE_I4(dest);

    Tac *t = allocateNewTac(Tac::LOAD_SYMBOL);
    t->op0.var = dest;
    t->op1.name = label;

    return t;
}

Tac *Tac::Load(Temp dest, Temp src, int offset) {
    REQUIRE_I4(dest);
    REQUIRE_I4(src);

    Tac *t = allocateNewTac(Tac::LOAD);
    t->op0.var = dest;
    t->op1.var = src;
    t->op1.offset = offset;
    return t;
}

Tac *Tac::Store(Temp src, Temp base, int offset) {
    REQUIRE_I4(src);
    REQUIRE_I4(base);

    Tac *t = allocateNewTac(Tac::STORE);
    t->op0.var = src;
    t->op1.var = base;
    t->op1.offset = offset;
    return t;
}

/* Creates a Jump tac.
 *
 * NOTE:
 *   jump to address
 * PARAMETERS:
 *   dest - destination address
 * RETURNS:
 *   a Jump tac
 */
Tac *Tac::Jump(Label dest) {
    Tac *t = allocateNewTac(Tac::JUMP);
    t->op0.label = dest;
    dest->target = true;

    return t;
}

/* Creates a JZero tac.
 *
 * NOTE:
 *   jump to address if condition is zero
 * PARAMETERS:
 *   dest - destination address
 *   cond - value of the condition expression
 * RETURNS:
 *   a JZero tac
 */
Tac *Tac::JZero(Label dest, Temp cond) {
    REQUIRE_I4(cond);

    Tac *t = allocateNewTac(Tac::JZERO);
    t->op0.label = dest;
    dest->target = true;
    t->op1.var = cond;

    return t;
}

/* Creates a Push tac.
 *
 * NOTE:
 *   push to stack
 * PARAMETERS:
 *   src  - source
 * RETURNS:
 *   a Push tac
 */
Tac *Tac::Push(Temp src) {
    REQUIRE_I4(src);

    Tac *t = allocateNewTac(Tac::PUSH);
    t->op0.var = src;

    return t;
}

/* Creates a Pop tac.
 *
 * NOTE:
 *   pop from stack
 * PARAMETERS:
 *   dest - result
 * RETURNS:
 *   a Pop tac
 */
Tac *Tac::Pop(Temp dest) { // dest may be NULL
    REQUIRE_I4(dest);

    Tac *t = allocateNewTac(Tac::POP);
    t->op0.var = dest;

    return t;
}

Tac *Tac::Param(Temp dest) {
    REQUIRE_I4(dest);

    Tac *t = allocateNewTac(Tac::PARAM);
    t->op0.var = dest;
    
    return t;
}

Tac *Tac::Alloc(Temp dest, int num) {
    REQUIRE_I4(dest);

    Tac *t = allocateNewTac(Tac::ALLOC);
    t->op0.var = dest;
    t->op1.size = num;
    
    return t;
}

/* Creates a Return tac.
 *
 * NOTE:
 *   return with value
 * PARAMETERS:
 *   value - return value
 * RETURNS:
 *   a Return tac
 */

Tac *Tac::Return(Temp value) {
    REQUIRE_I4(value);

    Tac *t = allocateNewTac(Tac::RETURN);
    t->op0.var = value;

    return t;
}

Tac *Tac::Call(Temp dest, Label label) {
    REQUIRE_I4(dest);

    Tac *t = allocateNewTac(Tac::CALL);
    t->op0.var = dest;
    t->op1.label = label;
    return t;
}

/* Creates a Mark tac.
 *
 * NOTE:
 *   mark label
 * PARAMETERS:
 *   label - the label to mark
 * RETURNS:
 *   a Mark tac
 */
Tac *Tac::Mark(Label label) {
    Tac *t = allocateNewTac(Tac::MARK);
    t->op0.label = label;
    label->where = t;

    return t;
}

/* Outputs a temporary variable.
 *
 * PARAMETERS:
 *   os   - the output stream
 *   v    - the temporary variable
 * RETURNS:
 *   the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Temp v) {
    return (os << "T" << v->id);
}

/* Outputs a label object.
 *
 * PARAMETERS:
 *   os   - the output stream
 *   l    - the label object
 * RETURNS:
 *   the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Label l) {
    if (l->str_form.empty())
        return (os << "__L" << l->id);
    else
        return (os << "_" << (l->str_form));
}

/* Outputs a functy object.
 *
 * PARAMETERS:
 *   os   - the output stream
 *   f    - the functy object
 * RETURNS:
 *   the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Functy f) {
    for (Tac *p = f->code; p != NULL; p = p->next)
        os << p;
    return os;
}

/* Dumps the Tac node to an output stream.
 *
 * PARAMETERS:
 *   os   - the output stream
 */
void Tac::dump(std::ostream &os) {
    switch (op_code) {
    case MEMO:
        os << "    memo '" << op0.memo << "'";
        break;

    case ASSIGN:
        os << "    " << op0.var << " <- " << op1.var;
        break;

    case ADD:
        os << "    " << op0.var << " <- (" << op1.var << " + " << op2.var
           << ")";
        break;

    case SUB:
        os << "    " << op0.var << " <- (" << op1.var << " - " << op2.var
           << ")";
        break;

    case MUL:
        os << "    " << op0.var << " <- (" << op1.var << " * " << op2.var
           << ")";
        break;

    case DIV:
        os << "    " << op0.var << " <- (" << op1.var << " / " << op2.var
           << ")";
        break;

    case MOD:
        os << "    " << op0.var << " <- (" << op1.var << " % " << op2.var
           << ")";
        break;

    case EQU:
        os << "    " << op0.var << " <- (" << op1.var << " == " << op2.var
           << ")";
        break;

    case NEQ:
        os << "    " << op0.var << " <- (" << op1.var << " != " << op2.var
           << ")";
        break;

    case LES:
        os << "    " << op0.var << " <- (" << op1.var << " < " << op2.var
           << ")";
        break;

    case LEQ:
        os << "    " << op0.var << " <- (" << op1.var << " <= " << op2.var
           << ")";
        break;

    case GTR:
        os << "    " << op0.var << " <- (" << op1.var << " > " << op2.var
           << ")";
        break;

    case GEQ:
        os << "    " << op0.var << " <- (" << op1.var << " <= " << op2.var
           << ")";
        break;

    case NEG:
        os << "    " << op0.var << " <- (- " << op1.var << ")";
        break;

    case LAND:
        os << "    " << op0.var << " <- (" << op1.var << " && " << op2.var
           << ")";
        break;

    case LOR:
        os << "    " << op0.var << " <- (" << op1.var << " || " << op2.var
           << ")";
        break;

    case LNOT:
        os << "    " << op0.var << " <- (! " << op1.var << ")";
        break;

    case BNOT:
        os << "    " << op0.var << " <- (~ " << op1.var << ")";
        break;

    case MARK:
        os << op0.label << ":";
        break;

    case JUMP:
        os << "    jump   " << op0.label;
        break;

    case JZERO:
        os << "    if (" << op1.var << " == 0) jump " << op0.label;
        break;

    case PUSH:
        os << "    push   " << op0.var;
        break;

    case POP:
        if (NULL != op0.var)
            os << "    " << op0.var << " <- pop()";
        else
            os << "    pop()";
        break;

    case RETURN:
        os << "    return " << op0.var;
        break;

    case LOAD_IMM4:
        os << "    " << op0.var << " <- " << op1.ival;
        break;
    
    case LOAD_SYMBOL:
        os << "    " << op0.var << " = LOAD_SYMBOL " << op1.name;
        break;
    
    case LOAD:
        os << "    " << op0.var << " = LOAD " << op1.var << ", " << op1.offset;
        break;

    case CALL:
        os << "    " << op0.var << " = CALL " << op1.label;
        break;

    case PARAM:
        os << "    PARAM " << op0.var;
        break;

    case STORE:
        os << "    STORE " << op0.var << " -> " << op1.var << ", " << op1.offset;
        break;
    
    case ALLOC:
        os << "    " << op0.var << " = ALLOC " << op1.size;
        break;

    default:
        mind_assert(false); // unreachable
        break;
    }
}

/* Dumps the Piece list to an output stream.
 *
 * PARAMETERS:
 *   os   - the output stream
 */
void Piece::dump(std::ostream &os) {
    for (Piece *p = this; p != NULL; p = p->next) {
        if (FUNCTY == p->kind)
            os << p->as.functy << std::endl;
    }
}

/* Outputs a single tac node.
 *
 * PARAMETERS:
 *   os   - the output stream
 *   t    - the tac node
 * RETURNS:
 *   the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Tac *t) {
    std::ostringstream oss;
    t->dump(oss);
    os << std::left << std::setw(32) << oss.str();

    // in dataflow analysis, we will print all the LiveOut sets
    if (Option::getLevel() == Option::DATAFLOW)
        os << "| " << t->LiveOut;

    os << std::endl;
    return os;
}
