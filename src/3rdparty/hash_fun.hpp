/*****************************************************
 *  Implementations of the hash functions.
 *
 *  The contents of this file are mainly from the SGI implementation
 *  of the C++ Standard Template Library.
 *  (http://www.sgi.com/tech/stl/)
 *
 *  We implemented our own version of hashed data structures because
 *  the implementation of hashed data structures are quite different
 *  in GCC 4 and MS VC 8.
 *
 *  Keltin Leung 
 */

// specialization of const char*
template<>
struct hash<const char*>
{	
  size_t
  operator()(const char* __s) const {
	unsigned long h = 0;
	for ( ; '\0' != *__s; ++ __s)
	  h = 5 * h + *__s;
	return size_t(h);
  }
};

// specialization of int
template<>
struct hash<int> { 
  size_t operator()(int __x) const {
	return (__x < 0 ? 3 - __x : __x);
  }
};

// specialization of unsigned int
template<>
struct hash<unsigned int> { 
  size_t operator()(unsigned int __x) const {
	return __x;
  }
};

// specialization of long
template<>
struct hash<long> {
  size_t operator()(long __x) const {
	return (__x < 0 ? 3 - __x : __x);
  }
};

// specialization of unsigned long
template<>
struct hash<unsigned long> {
  size_t operator()(unsigned long __x) const {
	return __x;
  }
};

// specialization of pointers
template<>
struct hash<void*> {
  size_t operator()(void* __x) const {
	return (long)__x;
  }
};
