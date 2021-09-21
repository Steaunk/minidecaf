/*****************************************************
 *  Offset Counter.
 * 
 */


#ifndef __MIND_OFFSETCOUNTER__
#define __MIND_OFFSETCOUNTER__

#include "define.hpp"

namespace mind {

#define MIND_OFFSETCOUNTER_DEFINED
namespace assembly {

/** Offset counter.
 *
 *  offset counters are used to compute target-machine-dependent offsets.
 */
class OffsetCounter {
  public:
    // offset kind
    typedef enum {
        GLOBAL = 0,   // for global variables
        LOCAL = 1,    // for local variables
        PARAMETER = 2 // for function parameters
    } Kind;

    // constructor
    OffsetCounter(int str[3], int dir[3]);
    // resets the specified counter to its default value
    void reset(Kind);
    // resets the specified counter to the specified value
    void reset(Kind, int);
    // gets the offset for a new unit with the specified size (and advance the
    // counter)
    int next(Kind, int);
    // gets the current value of the specified offset counter
    int get(Kind);
    // gets the default value of the specified offset counter
    int getDefault(Kind);
    // difference of two offset values
    static int diff(int, int);

  private:
    int current[3];   // current values
    int start[3];     // default values
    int direction[3]; // growth directions (1 or -1)
};

} // namespace assembly
} // namespace mind

#endif // __MIND_OFFSETCOUNTER__
