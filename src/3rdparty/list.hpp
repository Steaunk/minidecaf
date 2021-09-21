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
 *    reverse_iterator
 *      - reverse iterator type
 *
 *	  List()
 *      - default constructor
 *
 *	  List(const _T&)
 *      - constructs a list with one element
 *
 *	  append(const _T&)
 *      - appends an element to the list
 *
 *    addAtHead(const _T&)
 *      - adds an element at the head of the list
 *
 *    concate(List<_T>*)
 *      - concates an other list
 *
 *    insertBefore(const _T&, iterator)
 *      - inserts an element before the given iterator
 *
 *    empty(void) const
 *      - whether it is an empty list
 *
 *    length(void) const
 *      - length of the list
 *
 *    begin(void)
 *      - gets an iterator pointing to the first element
 *
 *    end(void)
 *      - gets an iterator pointing beyond the last element
 *
 *    rbegin(void)
 *      - gets an reverse iterator pointing to the last element
 *
 *    rend(void)
 *      - gets an reverse iterator pointing beyond the first element
 *
 *  Keltin Leung 
 */

#ifndef __MIND_LIST__
#define __MIND_LIST__

#include "boehmgc.hpp"

#include <list>
#include <iostream>

namespace mind {

  namespace util {

	// List template class (just a wrapper)
	template <typename _T>
	class List {
	private:
	  typedef std::list<_T, gc_allocator<_T> >  _Cntner;
	  _Cntner _l;
	  
	public:
	  typedef typename _Cntner::iterator         iterator;
	  typedef typename _Cntner::reverse_iterator reverse_iterator;

	  List() {}

	  List(const _T& e) { append(e); }
	  
	  // Appends an element to the list
	  void     append(const _T& e) {
		_l.push_back (e);
	  }
	  
	  // Inserts an element at the head of the list
	  void     addAtHead(const _T& e) {
		_l.push_front (e);
	  }
	  
	  // Concates the list with another list
	  void     concate(List<_T>* l) {
		_l.merge (l->_l);
	  }
	  
	  // Inserts an element before the specified position
	  void     insertBefore(const _T& e, iterator i) {
		_l.insert (i, e);
	  }
	  
	  // Determines whether the list is empty
	  bool     empty(void) const {
		return _l.empty();
	  }

	  // Gets the length of the list
	  size_t   length(void) const {
		return _l.size();
	  }
	  
	  // Gets the iterator pointing to the first element
	  iterator begin(void) {
		return _l.begin();
	  }
	  
	  // Gets the iterator pointing beyond the last element
	  iterator end(void) {
		return _l.end();
	  }

	  reverse_iterator rbegin(void) {
		return _l.rbegin();
	  }

	  reverse_iterator rend(void) {
		return _l.rend();
	  }

	  virtual ~List() {}
	};

  }  
}

#endif // __MIND_LIST__
