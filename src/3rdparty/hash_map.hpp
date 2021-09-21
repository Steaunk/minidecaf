/*****************************************************
 *  Implementation of the hashed map.
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
HashMap<_K, _V, _H>::HashMap()
  : _ht(100) {
}
  
template <typename _K, typename _V, typename _H>
HashMap<_K, _V, _H>::HashMap(size_type __n)
  : _ht(__n) {
}

template <typename _K, typename _V, typename _H>
typename HashMap<_K, _V, _H>::size_type
HashMap<_K, _V, _H>::size() const {
  return _ht.size();
}
      
template <typename _K, typename _V, typename _H>
bool
HashMap<_K, _V, _H>::empty() const {
  return _ht.empty();
}
  
template <typename _K, typename _V, typename _H>
typename HashMap<_K, _V, _H>::iterator
HashMap<_K, _V, _H>::begin() {
  return _ht.begin();
}

template <typename _K, typename _V, typename _H>
typename HashMap<_K, _V, _H>::iterator
HashMap<_K, _V, _H>::end() {
  return _ht.end();
}

template <typename _K, typename _V, typename _H>
std::pair<typename HashMap<_K, _V, _H>::iterator, bool>
HashMap<_K, _V, _H>::insert(const value_type& __obj) {
  return _ht.insert_unique(__obj);
}

template <typename _K, typename _V, typename _H>
typename HashMap<_K, _V, _H>::iterator
HashMap<_K, _V, _H>::find(const key_type& __key) {
  return _ht.find(__key);
}

template <typename _K, typename _V, typename _H>
_V&
HashMap<_K, _V, _H>::operator[](const key_type& __key) {
  return _ht.find_or_insert(value_type(__key, _V())).second;
}

template <typename _K, typename _V, typename _H>
typename HashMap<_K, _V, _H>::size_type
HashMap<_K, _V, _H>::count(const key_type& __key) const	{
  return _ht.count(__key);
}

template <typename _K, typename _V, typename _H>
typename HashMap<_K, _V, _H>::size_type
HashMap<_K, _V, _H>::erase(const key_type& __key) {
  return _ht.erase(__key);
}

template <typename _K, typename _V, typename _H>
void
HashMap<_K, _V, _H>::clear() {
  _ht.clear();
}

template <typename _K, typename _V, typename _H>
void
HashMap<_K, _V, _H>::resize(size_type __hint) {
  _ht.resize(__hint);
}
