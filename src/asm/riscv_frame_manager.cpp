/*****************************************************
 *  Implementation of the RISC-V Stack-frame Manager.
 *
 *
 */

#include "asm/riscv_frame_manager.hpp"
#include "config.hpp"
#include "tac/tac.hpp"

#include <algorithm>
#include <cstring>

using namespace mind::assembly;
using namespace mind::tac;
using namespace mind::util;
using namespace mind;

#define WORD_SIZE 4
#define UPDATE_MAX()                                                           \
    { max_size = (max_size < size ? size : max_size); }

/* Constructor.
 *
 * PARAMETERS:
 *   start - the start offset
 */
RiscvStackFrameManager::RiscvStackFrameManager(int start) {
    start_offset = start;
    reserved_size = size = max_size = 0;
    capacity = 4;
    slots = new Temp[capacity];
    std::memset(slots, 0, capacity * sizeof(Temp));
}

/* Resets the temporary variable area.
 *
 * This function will reset the stackframe
 * so that it contains only the reserved area.
 */
void RiscvStackFrameManager::reset(void) {
    for (int i = reserved_size; i < size; ++i) {
        slots[i]->is_offset_fixed = false;
    }

    size = reserved_size;
}

/* Reserves a temporary variable in the reserved area.
 *
 * PARAMETERS:
 *   v     - the temporary variable to reserve
 */
void RiscvStackFrameManager::reserve(Temp v) {
    if (v->is_offset_fixed)
        return;

    ensureCapacity();
    v->offset = offsetOf(size);
    v->is_offset_fixed = true;
    slots[size] = v;
    size = reserved_size = reserved_size + 1;
    UPDATE_MAX();
}

/* Finds a slot with the given variable as its content.
 *
 * PARAMETERS:
 *   v     - the temporary to look for
 * RETURNS:
 *   the slot number if found, else -1 will be returned.
 */
int RiscvStackFrameManager::findSlotOf(Temp v) {
    mind_assert(NULL != v && (WORD_SIZE == v->size));

    int i = -1;

    for (i = size - 1; i >= 0; --i)
        if (slots[i] == v)
            break;

    return i;
}

/* Gets a slot to save the specified variable.
 *
 * PARAMETERS:
 *   v        - the variable to save
 *   liveness - the set of the current alive variables
 * RETURNS:
 *   a slot number representing the slot into which
 *   the variable can be safely saved.
 */
int RiscvStackFrameManager::getSlotToWrite(Temp v, Set<Temp> *liveness) {
    mind_assert(NULL != v && NULL != liveness && !v->is_offset_fixed);

    int i = findSlotOf(v);

    if (i >= 0) {
        v->offset = offsetOf(i);
        v->is_offset_fixed = true;
        return i;
    }

    // now we should look for an available slot
    for (i = reserved_size; i < size; ++i) {
        if (!liveness->contains(slots[i])) {
            // ok, let's drive it out
            slots[i]->is_offset_fixed = false;
            v->offset = offsetOf(i);
            v->is_offset_fixed = true;
            slots[i] = v;
            return i;
        }
    }

    // if all slots are live, get a new slot
    ensureCapacity();
    v->offset = offsetOf(size);
    v->is_offset_fixed = true;
    slots[size] = v;
    ++size;
    UPDATE_MAX();
    return (size - 1);
}

/* Gets the suitable stack-frame size.
 *
 * NOTE: this size is used to allocate the stack-frame when entering a function.
 *
 * RETURNS:
 *   the minimum stack-frame size without violating the other data
 *   (excluding the $fp and $ra area)
 */
int RiscvStackFrameManager::getStackFrameSize(void) {
    return (max_size * WORD_SIZE);
}

/* Computes the offset of a specified slot.
 *
 * PARAMETERS:
 *   slot_num - slot number
 * RETURNS:
 *   the memmory offset of this slot (from the beginning of the stack-frame)
 */
int RiscvStackFrameManager::offsetOf(int slot_num) {
    return (start_offset - slot_num * WORD_SIZE);
}

/* Ensures the capacity of the underlying buffer.
 */
void RiscvStackFrameManager::ensureCapacity(void) {
    if (capacity < size + 4) {
        capacity = capacity * 2;
        Temp *new_slots = new Temp[capacity];
        std::copy(slots, slots + size, new_slots);
        slots = new_slots;
    }
}
