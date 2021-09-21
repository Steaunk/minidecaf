/*****************************************************
 *  Garbage-collectable Data Structure: List.
 *
 *  NOTE: it is different from std::list.
 *        it is used as ast::XXXXList.
 *
 *  PUBLIC INTERFACES:
 *    iterator
 *      - iterator type
 *
 *    const_iterator
 *      - const iterator type
 *
 *    Set()
 *      - default constructor
 *
 *    Set(size_t capacity)
 *      - constructor (with capacity of the underlying container specified)
 *
 *    Set(const _T e)
 *      - constructs a set with a single element
 *
 *    Set(const set_type& s)
 *      - constructs a set as same as the given set
 *
 *    size_t size(void) const
 *      - returns the size of this set
 *
 *    void add(const _T e)
 *      - adds an element to this set
 *
 *    void remove(const _T e)
 *      - removes an element from this set
 *
 *	  bool contains(const _T e) const
 *      - tests whether this set contains the specified element
 *
 *    bool empty(void) const
 *      - whether it is an empty set
 *
 *    void clear(void)
 *      - erases all the elements in this set
 *
 *	  Set<_T>* unionWith(const Set<_T>* s) const
 *      - gets the union of this set and set s (i.e. this \cup s)
 *
 *	  Set<_T>* intersectionWith(const Set<_T>* s) const
 *      - gets the intersection of this set and set s (i.e. this \cap s)
 *
 *	  Set<_T>* differenceFrom(const Set<_T>* s) const
 *      - gets the difference set from set s (i.e. this - s)
 *
 *    Set<_T>* clone(void)
 *      - clones this set
 *
 *	  bool equal(const Set<_T>* s)
 *      - tests whether this set is equal to the given set
 *	  
 *    iterator begin(void)
 *      - gets the begin iterator
 *
 *    iterator end(void)
 *      - gets the end iterator
 *
 *    const_iterator begin(void) const
 *      - gets the begin const iterator
 *
 *    const_iterator end(void) const
 *      - gets the end const iterator
 *
 *  Keltin Leung 
 */

#ifndef __MIND_SET__
#define __MIND_SET__

#include "boehmgc.hpp"

#include <algorithm>

namespace mind {

  namespace util {

	template <typename _T>
	class Set {
	private:
	  size_t _size;
	  size_t _capacity;
	  _T*    _container;

	  void   _ensureCapacity(void) {
		if (_size + 3 < _capacity/2)
		  _capacity = _size + 3;
		else if (_size >= _capacity)
		  _capacity = _size*2 + 3;
		else
		  return;

		_T* new_container = new _T[_capacity];
		std::copy(_container, _container+_size, new_container);
		if (NULL != _container)
		  delete[] _container;
		_container = new_container;
	  }
	  
	public:
	  typedef _T*           iterator;
	  typedef const _T*     const_iterator;
	  typedef Set<_T>       set_type;

	  Set() {
		_size = 0;
		_capacity = 3;
		_container = new _T[_capacity];
	  }

	  Set(size_t capacity) {
		_size = 0;
		_capacity = std::max(capacity, 3ul);
		_container = new _T[_capacity];
	  }

	  Set(const _T e) {
		_size = 1;
		_capacity = 3;
		_container = new _T[_capacity];
		_container[0] = e;
	  }

	  Set(const set_type& s) {
		_size = s._size;
		_capacity = s._capacity;
		_container = new _T[_capacity];
		std::copy(s.begin(), s.end(), begin());
	  }

	  ~Set() {
		if (NULL != _container)
		  delete[] _container;
	  }

	  size_t size(void) const {
		return _size;
	  }
	  
	  void add(const _T e) {
		if (! contains(e)) {
		  _ensureCapacity();
		  
		  int i = _size - 1;
		  while (i >= 0 && e < _container[i]) {
			_container[i+1] = _container[i];
			-- i;
		  }
		  
		  _container[i+1] = e;
		  ++ _size;
		}
	  }

	  void remove(const _T e) {
		_T* p = std::lower_bound(begin(), end(), e);

		if (*p == e) {
		  std::copy(p+1, end(), p);
		  --_size;
		}
	  }

	  bool empty(void) const {
		return (0 == _size);
	  }
	  
	  bool contains(const _T e) const {
		const _T* p = std::lower_bound(begin(), end(), e);

		return (*p == e);
	  }

	  void clear(void) {
		_size = 0;
		// we don't release the memory here
	  }

	  set_type* unionWith(const set_type* s) const {
		set_type* tmp = new set_type(_size + s->_size + 3);

		iterator i = std::set_union(begin(), end(), s->begin(), s->end(), tmp->begin());
		tmp->_size = i - tmp->begin();
		tmp->_ensureCapacity();  // may reduce the capacity

		return tmp;
	  }

	  set_type* intersectionWith(const set_type* s) const {
		set_type* tmp = new set_type(std::min(_size, s->_size) + 3);

		iterator i = std::set_intersection(begin(), end(), s->begin(), s->end(), tmp->begin());
		tmp->_size = i - tmp->begin();
		tmp->_ensureCapacity();  // may reduce the capacity

		return tmp;
	  }

	  set_type* differenceFrom(const set_type* s) const {
		set_type* tmp = new set_type(_size + 3);

		iterator i = std::set_difference(begin(), end(), s->begin(), s->end(), tmp->begin());
		tmp->_size = i - tmp->begin();
		tmp->_ensureCapacity();  // may reduce the capacity

		return tmp;
	  }

	  set_type* clone(void) {
		return new set_type(*this);
	  }

	  bool equal(const set_type* s) {
		if (_size != s->_size) {
		  return false;
		  
		} else {
		  for (size_t i=0; i<_size; ++i)
			if (_container[i] != s->_container[i])
			  return false;

		  return true;
		}
	  }
	  
	  iterator begin(void) {
		return _container;
	  }
	  
	  iterator end(void) {
		return (_container + _size);
	  }

	  const_iterator begin(void) const {
		return _container;
	  }

	  const_iterator end(void) const {
		return (_container + _size);
	  }

	};

  }  
}

#endif // __MIND_SET__
