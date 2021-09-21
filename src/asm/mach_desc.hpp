/*****************************************************
 *  Machine Description (interface).
 *
 */

#ifndef __MIND_MACHDESC__
#define __MIND_MACHDESC__

#include "define.hpp"

#include <iostream>

namespace mind {
#define MIND_MACHDESC_DEFINED
namespace assembly {
/**
 * Interface of ``instruction''.
 *
 * NOTE: used by BasicBlock for multi-target support.
 */
struct Instr {
    bool cancelled; // whether it is cancelled by peephole optimizer
};

/**
 * Interface of ``machine description''.
 *
 * Machine description is used to provide infomation about the target machine.
 */
class MachineDesc {
  public:
    // gets the offset counter for this machine
    virtual OffsetCounter *getOffsetCounter(void) = 0;
    // translates Tac sequences into assembly code (and output)
    virtual void emitPieces(scope::GlobalScope *, tac::Piece *, std::ostream &) = 0;
    // destructor
    virtual ~MachineDesc() {}
};

} // namespace assembly
} // namespace mind

#endif // __MIND_MACHDESC__
