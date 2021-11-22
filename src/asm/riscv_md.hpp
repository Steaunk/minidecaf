/*****************************************************
 *  RISC-V Machine Description.
 *
 */

#ifndef __MIND_RISCVMD__
#define __MIND_RISCVMD__

#include "3rdparty/set.hpp"
#include "asm/mach_desc.hpp"
#include "asm/riscv_frame_manager.hpp"
#include "define.hpp"

namespace mind {
#define RISCV_COMPONENTS_DEFINED
namespace assembly {
// for convinience
typedef util::Set<tac::Temp> LiveSet;

/**
 * RISC-V register.
 *
 * NOTE: we regard all general-purpose registers as caller-saved
 *
 */
struct RiscvReg {
    enum {
        ZERO = 0,
        RA,
        SP,
        GP,
        TP,
        T0,
        T1,
        T2,
        FP,
        S1,
        A0,
        A1,
        A2,
        A3,
        A4,
        A5,
        A6,
        A7,
        S2,
        S3,
        S4,
        S5,
        S6,
        S7,
        S8,
        S9,
        S10,
        S11,
        T3,
        T4,
        T5,
        T6,
        TOTAL_NUM // total number to registers
    };

    const char *name; // register name
    tac::Temp var;    // associated variable
    bool dirty;       // whether it is out of sychronized with the memory
    bool general;     // whether it is a generl-purpose register

    // two constructors for convenience
    RiscvReg(const char *reg_name, bool is_general);
    RiscvReg() {}
};

/**
 * RISC-V instruction.
 *
 * NOTE:
 *   1. it represents only the instructions used inside a basic block
 *   2. not all instructions are modeled (e.g. floting-point arithmetic instructions)
 * 
 */
struct RiscvInstr : public Instr {
    enum OpCode {
        // assembler directives
        COMMENT,
        // instructions/pseudo instructions
        AND,
        OR,
        XOR,
        ADD,
        ADDI,
        SUB,
        MUL,
        DIV,
        REM,
        NEG,
        J,
        BEQZ,
        RET,
        LW,
        LI,
        SW,
        MOVE,
        NOT, //BNOT
        SEQZ, //LNOT
        SNEZ, 
        SLT,
        SLTU,
        SGT,
        SGE,
        CALL,
        LA
        // You could add other instructions/pseudo instructions here
    } op_code; // operation code

    RiscvReg *r0, *r1, *r2; // 3 register operands
    int i;                  // offset or immediate number
    std::string l;          // target label. for LA, B, BEQZ or JAL
    const char *comment;    // comment in this line

    RiscvInstr *next; // next instruction

    // "cancelled" field is inherited from assembly::Instr.
};

/**
 * RISC-V machine description.
 *
 * The main function of RiscvDesc is to translate a "Piece" into assembly code.
 */
class RiscvDesc : public MachineDesc {
  public:
    // constructor
    RiscvDesc();
    // gets the offset counter for RISC-V
    virtual OffsetCounter *getOffsetCounter(void);
    // translates the given "tac::Piece" into RISC-V assembly code
    virtual void emitPieces(scope::GlobalScope *, tac::Piece *,
                            std::ostream &os);

  private:
    // where to output the assembly code
    std::ostream *_result;
    // riscv offset counter
    OffsetCounter *_counter;
    // auxilliary field for addInstr
    RiscvInstr *_tail;
    // stack-frame manager for `register spilling`
    RiscvStackFrameManager *_frame;
    // label counter for allocating new labels
    int _label_counter;
    
    // allocates a new label
    const char *getNewLabel(void);
    // translates the tac_chain of a basic block into the instr_chain
    RiscvInstr *prepareSingleChain(tac::BasicBlock *, tac::FlowGraph *);

    // translates a TAC into assembly instructions
    void emitTac(tac::Tac *);
    // translates a LoadImm4 TAC into assembly instructions
    void emitLoadImm4Tac(tac::Tac *);
    // translates a LoadSymbol TAC into assembly instructions
    void emitLoadSymbolTac(tac::Tac *);
    // translates a Load TAC into assembly instructions
    void emitLoadTac(tac::Tac *);
    // translates a Store TAC into assembly instructions
    void emitStoreTac(tac::Tac *);
    // translates a Unary TAC into assembly instructions
    void emitUnaryTac(RiscvInstr::OpCode, tac::Tac *);
    // translates a Binary TAC into assembly instructions
    void emitBinaryTac(RiscvInstr::OpCode, tac::Tac *);
    // translates a Push TAC into assembly instructions
    void emitPushTac(tac::Tac *);
    // translates a Call TAC into assembly instructions
    void emitCallTac(tac::Tac *);
    
    void emitAllocTac(tac::Tac *);

    // outputs an instruction
    void emit(std::string, const char *, const char *);
    // outputs a function
    void emitFuncty(tac::Functy);
    // prints the leading code of a function
    void emitProlog(tac::Label, int);
    // prints the assembly code of a single trace
    void emitTrace(tac::BasicBlock *, tac::FlowGraph *);
    // prints a single RISC-V instruction
    void emitInstr(RiscvInstr *);
    // appends a new instruction to "_tail"
    void addInstr(RiscvInstr::OpCode, RiscvReg *, RiscvReg *, RiscvReg *, int,
                  std::string, const char *);


    /*** sketch for peephole optimizer (inside a basic block) ***/
    void simplePeephole(RiscvInstr *);


    /*** the register allocator ***/
    RiscvReg *_reg[RiscvReg::TOTAL_NUM]; // registers of a machine
    int _lastUsedReg;                    // which register was used last?

    // acquires a register to read the value of a variable
    int getRegForRead(tac::Temp, int, LiveSet *);
    // acquires a register to write the value of a variable
    int getRegForWrite(tac::Temp, int, int, LiveSet *);
    // spills the content of a register into memory
    void spillReg(int, LiveSet *);
    // spills all dirty registers into memory (for control-flow change)
    void spillDirtyRegs(LiveSet *);
    // looks up a register holding the specified variable
    int lookupReg(tac::Temp);
    // selects a register to spill into memory
    int selectRegToSpill(int, int, LiveSet *);
};

} // namespace assembly
} // namespace mind

#endif // __MIND_RISCVMD__
