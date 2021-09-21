/*****************************************************
 *  Implementation of the hash table iterator.
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

template <typename _K, typename _V, typename _H>
_HashIterator<_K, _V, _H>::_HashIterator(_Node* __n, _HashTable* __tab)
  : _cur(__n), _ht(__tab) {
}
  
template <typename _K, typename _V, typename _H>
_HashIterator<_K, _V, _H>::_HashIterator() {
}
  
template <typename _K, typename _V, typename _H>
typename _HashIterator<_K, _V, _H>::reference
_HashIterator<_K, _V, _H>::operator*() const {
  return _cur->_val;
}
  
template <typename _K, typename _V, typename _H>
typename _HashIterator<_K, _V, _H>::pointer
_HashIterator<_K, _V, _H>::operator->() const {
  return &(operator*());
}
  
template <typename _K, typename _V, typename _H>
bool
_HashIterator<_K, _V, _H>::operator==(const iterator& __it) const {
  return _cur == __it._cur;
}
  
template <typename _K, typename _V, typename _H>
bool
_HashIterator<_K, _V, _H>::operator!=(const iterator& __it) const {
  return _cur != __it._cur;
}

template <typename _K, typename _V, typename _H>
_HashIterator<_K, _V, _H>&
_HashIterator<_K, _V, _H>::operator++() {
  const _Node* _old = _cur;
  _cur = _cur->_next;
  if (NULL == _cur) {
	size_type _n = _ht->_nbkt(_old->_val);
	while (NULL == _cur && ++_n < _ht->_num_buckets)
	  _cur = _ht->_buckets[_n];
  }
  return *this;
}

template <typename _K, typename _V, typename _H>
inline _HashIterator<_K, _V, _H>
_HashIterator<_K, _V, _H>::operator++(int) {
  iterator __tmp = *this;
  ++ *this;
  return __tmp;
}
