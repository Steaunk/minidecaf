/*****************************************************
 *  Implementation of OffsetCounter.
 *
 *
 *  Keltin Leung 
 */

#include "asm/offset_counter.hpp"
#include "config.hpp"

using namespace mind::assembly;

/* Constructor.
 *
 * PARAMETERS:
 *   str  - default offsets of the three kinds
 *   dir  - growth directions of the three kinds
 * NOTE:
 *   this constructor should be invoked only in machine descriptions
 */
OffsetCounter::OffsetCounter(int str[3], int dir[3]) {
    start[0] = str[0];
    start[1] = str[1];
    start[2] = str[2];

    direction[0] = dir[0];
    direction[1] = dir[1];
    direction[2] = dir[2];
}

/* Reset the specified counter to its default value.
 *
 * PARAMETERS:
 *   k    - offset kind
 */
void OffsetCounter::reset(OffsetCounter::Kind k) { current[k] = start[k]; }

/* Reset the specified counter to a specified value.
 *
 * PARAMETERS:
 *   k     - offset kind
 *   value - the specified value (in bytes)
 */
void OffsetCounter::reset(OffsetCounter::Kind k, int value) {
    current[k] = value;
}

/* Gets the offset for a new unit with the specified size (and advance the
 * counter).
 *
 * PARAMETERS:
 *   k     - offset kind
 *   bytes - bytes to grow
 */
int OffsetCounter::next(OffsetCounter::Kind k, int bytes) {
    int tmp = current[k];

    current[k] += direction[k] * bytes;
    if (direction[k] < 0)
        tmp = current[k];

    return tmp;
}

/* Gets the current value of the specified counter
 *
 * PARAMETERS:
 *   k     - offset kind
 * RETURNS:
 *   current offset value in the counter
 */
int OffsetCounter::get(OffsetCounter::Kind k) { return current[k]; }

/* Gets the default value of the specified offset counter.
 *
 * PARAMETERS:
 *   k     - offset kind
 * RETURNS:
 *   the default value of the specified kind of offset
 */
int OffsetCounter::getDefault(OffsetCounter::Kind k) { return start[k]; }

/* Computes the difference of two offsets.
 *
 * PARAMETERS:
 *   a     - offset 1
 *   b     - offset 2
 * RETURNS:
 *   the differences of the given offsets
 */
int OffsetCounter::diff(int a, int b) { return (a < b ? b - a : a - b); }
