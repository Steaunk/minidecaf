/*****************************************************
 *  Hans J. Boehm Garbage Collector Header File
 *  (http://www.hpl.hp.com/personal/Hans_Boehm/gc/)
 *
 *  Keltin Leung 
 *
 *  NOTE: In order to enable the garbage collector
 *        functions, please include "config.hpp" as
 *        the first line in EACH of your *.cpp files.
 */

#ifndef __MIND_BOEHMGC__
#define __MIND_BOEHMGC__

#include <gc/gc_allocator.h>
#include <gc/gc.h>
#include <new>

/* The overloaded "new" operator.
 */
inline void* operator new(size_t n) {
  return GC_malloc(n);
}

/* The overloaded "new[]" operator.
 */
inline void* operator new[](size_t n) {
  return GC_malloc(n);
}

/* The overloaded "delete" operator.
 */
inline void operator delete(void *) {
  // We will leave the work to the garbage collector
}

/* The overloaded "delete[]" operator.
 */
inline void operator delete[](void *) {
  // We will leave the work to the garbage collector
}

#define __GC

#endif /* __MIND_BOEHMGC__ */
