/*****************************************************
 *  In tac.hpp, we define Tac's data structures
 *
 */

#ifndef __MIND_TAC__
#define __MIND_TAC__

#include "3rdparty/set.hpp"
#include "define.hpp"

#include <iostream>

namespace mind {

#define MIND_TAC_DEFINED
namespace tac {
/** Representation of a Temp.
 *
 *  NOTE: We typedef TempObject* as Temp, for easy coding & code reading.
 *        So use Temp instead of directly using TempObject*
 *        eg. Define a new Temp: Temp t = new TempObject();
 *
 *        An example for Temp's offset on the stack: offset of Temp1 is 8.
 *        If you want to access Temp1, its address is 8(fp).
 *        |     Temp1      |
 *        |    old fp      |
 *        |      ra        | <- fp (bottom of the stack)
 *
 */
typedef struct TempObject {
    int id;               // id of a Temp (Temp1, ... , TempN)
    int size;             // size of a Temp (e.g. size = 4 for int32)
    bool is_offset_fixed; // whether the Temp has been allocated on the stack
    int offset;           // the offset on the stack (relative to fp, see the example)
} * Temp;

/** Representation of a Label.
 *
 *  NOTE: Similar to Temp, define a new Label: Label l = new LabelObject();
 *
 */
typedef struct LabelObject {
    int id;               // id of a Label
    std::string str_form; // string format of a Label
    bool target;          // whether it is a target (eg. JUMP <Label>)
    Tac *where;           // The TAC which marks this Label (for dataflow analysis)
} * Label;

/** Representation of a function.
 *
 *  NOTE: Don't get confused with symb::Function -- Functy is for backend
 *        Don't use "FunctyObject" directly. Please use "Functy".
 */
typedef struct FunctyObject {
    Label entry;         // entry label of the function
    Tac *code;           // tac chain of the function
} * Functy;

typedef struct GlobalObject {
    std::string name;   
    int value;
    int size;
} *GlobalVar;

/** Three address code.
 *
 *  NOTE: We use "struct" instead of "class" here for your convenience.
 * 
 */
struct Tac {
    // Kinds of TACs.
    /**
     * If you want add your own Tac, specify the Tac type here,
     * and then design a static creation method for your Tac like we have provided in the framework
    */
    typedef enum {
        ASSIGN,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        EQU,
        NEQ,
        LES,
        LEQ,
        GTR,
        GEQ,
        NEG,
        LAND,
        LOR,
        LNOT,
        BNOT,
        MARK,
        JUMP,
        JZERO,
        PUSH,
        POP,
        RETURN,
        LOAD_IMM4,
        LOAD_SYMBOL,
        LOAD,
        STORE,
        MEMO,
        CALL,
        PARAM,
        ALLOC,
    } Kind;

    // Operand type
    typedef struct {
        Temp var;
        Label label;
        int ival;         // integer constant
        int offset;       // offset of parameter
        int size;         // stack frame size (for array allocation)
        std::string name; // symbol name (for LoadSymbol tac only)
        const char *memo; // memorandum (for Memo tac only)
    } Operand;

    Kind op_code; // operation kind
    Operand op0;  // usually serving as "dest"
    Operand op1;  // usually serving as "src1"
    Operand op2;  // usually serving as "src2"

    Tac *prev; // the previous tac
    Tac *next; // the next tac

    int bb_num; // basic block number, for dataflow analysis
    util::Set<Temp> *LiveOut; // for dataflow analysis: LiveOut set of this TAC
    int mark;   // auxiliary: do anything you want

    // static creation methods for TACs. (see: TransHelper)
    static Tac *Add(Temp dest, Temp op1, Temp op2);
    static Tac *Sub(Temp dest, Temp op1, Temp op2);
    static Tac *Mul(Temp dest, Temp op1, Temp op2);
    static Tac *Div(Temp dest, Temp op1, Temp op2);
    static Tac *Mod(Temp dest, Temp op1, Temp op2);
    static Tac *Equ(Temp dest, Temp op1, Temp op2);
    static Tac *Neq(Temp dest, Temp op1, Temp op2);
    static Tac *Les(Temp dest, Temp op1, Temp op2);
    static Tac *Leq(Temp dest, Temp op1, Temp op2);
    static Tac *Gtr(Temp dest, Temp op1, Temp op2);
    static Tac *Geq(Temp dest, Temp op1, Temp op2);
    static Tac *LAnd(Temp dest, Temp op1, Temp op2);
    static Tac *LOr(Temp dest, Temp op1, Temp op2);
    static Tac *Assign(Temp dest, Temp src);
    static Tac *Neg(Temp dest, Temp src);
    static Tac *LNot(Temp dest, Temp src);
    static Tac *BNot(Temp dest, Temp src);
    static Tac *LoadImm4(Temp dest, int value);
    static Tac *LoadSymbol(Temp dest, std::string label);
    static Tac *Load(Temp dest, Temp src, int offset);
    static Tac *Store(Temp src, Temp base, int offset);
    static Tac *Jump(Label dest);
    static Tac *JZero(Label dest, Temp cond);
    static Tac *Pop(Temp dest);
    static Tac *Push(Temp src);
    static Tac *Return(Temp value);
    static Tac *Mark(Label label);
    static Tac *Memo(const char *);
    static Tac *Call(Temp dest, Label label);
    static Tac *Param(Temp dest);
    static Tac *Alloc(Temp dest, int num);

    // dumps a single tac node to some output stream
    void dump(std::ostream &);
};

/** Representation of the whole program.
 *
 *  NOTE: 
 *  A Program is modeled as a piece of functions (functy).
 *  Of course, if you want support more program structures,
 *  you can modify the `as` part.
 * 
 *  for example, add `vtable` to support class definition 
 *  (oops, it is beyond our requirements, only for your own interests)
 */
struct Piece {
    // kind of this Piece node
    enum {
        FUNCTY,
        GLOBAL
    } kind;

    // data of this Piece node
    union {
        Functy functy;
        GlobalVar globalVar;
    } as;

    // next Piece node
    Piece *next;

    // dumps the entire Piece list (not a single node)
    void dump(std::ostream &);
};
} // namespace tac
} // namespace mind

#endif // __MIND_TAC__
