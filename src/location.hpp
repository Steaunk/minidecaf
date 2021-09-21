/*****************************************************
 *  Location Record.
 *
 *
 *  Keltin Leung 
 */

#ifndef __MIND_LOCATION__
#define __MIND_LOCATION__

#include <iostream>

namespace mind {

#define MIND_LOCATION_DEFINED
struct Location {
    int line;
    int col;

    Location(int l, int c) : line(l), col(c) {}

    Location(int l) : line(l), col(-1) {}
};

inline bool operator<(const Location &lhs, const Location &rhs) {
    return lhs.line < rhs.line || (lhs.line == rhs.line && lhs.col < rhs.col);
}

} // namespace mind

#endif // __MIND_LOCATION__
