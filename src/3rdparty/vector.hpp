/*****************************************************
 *  Garbage-collectable Data Structure: Vector.
 *
 *  NOTE: just treat it like std::vector.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_VECTOR__
#define __MIND_VECTOR__

#include "boehmgc.hpp"

#include <vector>

namespace mind {

  namespace util {

	// Vector (garbage-collectable version)
	template <typename _T>
	class Vector : public std::vector<_T, gc_allocator<_T> > {
	};

  }  
}

#endif // __MIND_VECTOR__
