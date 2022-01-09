/*****************************************************
 *  AST Translation Helper.
 *
 *  NOTE: You should use this class to generate Tac's.
 *        Please don't use Tac::XXX directly.
 *
 */

#ifndef __MIND_TRANSHELPER__
#define __MIND_TRANSHELPER__

#include "define.hpp"
#include "tac/tac.hpp"

namespace mind {

#define MIND_TRANSHELPER_DEFINED
namespace tac {

/** Translation helper.
 *
 *  We use a helper to generate Tac's instead of calling the
 *  creationg functions in Tac.
 */
class TransHelper {
  public:
    // constructor
    TransHelper(assembly::MachineDesc *);
    // gets the offset counter of the target machine
    assembly::OffsetCounter *getOffsetCounter(void);
    // allocates a new int32 temporary variable
    Temp getNewTempI4(void);
    Temp allocNewTempI4(int);
    // allocates a new label
    Label getNewLabel(void);
    // allocates a new entry Label object for function
    Label getNewEntryLabel(symb::Function *);
    // starts to translate a function
    void startFunc(symb::Function *);
    // ends translating a function
    void endFunc(void);

    void genGlobalVarible(std::string, int, int);

    // Arithmetic
    Temp genAdd(Temp, Temp);
    Temp genSub(Temp, Temp);
    Temp genMul(Temp, Temp);
    Temp genDiv(Temp, Temp);
    Temp genMod(Temp, Temp);
    Temp genEqu(Temp, Temp);
    Temp genNeq(Temp, Temp);
    Temp genLes(Temp, Temp);
    Temp genLeq(Temp, Temp);
    Temp genGtr(Temp, Temp);
    Temp genGeq(Temp, Temp);
    Temp genNeg(Temp);
    // Logical
    Temp genLAnd(Temp, Temp);
    Temp genLOr(Temp, Temp);
    Temp genLNot(Temp);
    // Bitwise
    Temp genBNot(Temp);
    // Memory Access
    Temp genPop(void);
    void genPush(Temp);
    Temp genCall(Label);
    void genParam(Temp);
    // Control-flow related
    void genJump(Label);
    void genJumpOnZero(Label, Temp);
    void genReturn(Temp);
    // Miscellaneous
    void genAssign(Temp, Temp);
    Temp genLoadImm4(int);
    Temp genLoadSymbol(std::string);
    Temp genLoad(Temp, int);
    void genStore(Temp, Temp, int);
    void genMarkLabel(Label);
    void genMemo(const char *);

    // gets the entire Piece list
    Piece *getPiece();

  private:
    // the machine description
    assembly::MachineDesc *mach;
    // the Piece list
    Piece head, *ptail;
    // the Tac list of a function
    Tac *tacs, *tacs_tail;
    // the current Function
    symb::Function *current;
    // counters for temporaries and labels (for their index)
    int var_count, label_count;
    // whether the startup code has been generated
    bool startup_ok;

    // appends a Tac to the Tac list
    void chainUp(Tac *);
    // gets a Memo about the Function object
    Tac *memoOf(symb::Function *);
};
} // namespace tac
} // namespace mind

#endif // __MIND_TRANSHELPER__
