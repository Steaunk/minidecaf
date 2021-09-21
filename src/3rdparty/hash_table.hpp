/*****************************************************
 *  Implementation of the hash table.
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

static const unsigned long __prime_list[28] = {
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long
__next_prime(unsigned long __n) {
  const unsigned long* __first = __prime_list;
  const unsigned long* __last = __prime_list + 28;
  const unsigned long* pos = std::lower_bound(__first, __last, __n);
  return pos == __last ? *(__last - 1) : *pos;
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::allocator_type
HashTable<_K, _V, _H>::_get_allocator() const {
  return _node_allocator; 
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::_Node*
HashTable<_K, _V, _H>::_get_node() {
  return _node_allocator.allocate(1);
}

template <typename _K, typename _V, typename _H>
void
HashTable<_K, _V, _H>::_put_node(_Node* __p) {
  _node_allocator.deallocate(__p, 1);
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::size_type
HashTable<_K, _V, _H>::_next_size(size_type __n) const {
  return __next_prime(__n);
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::size_type
HashTable<_K, _V, _H>::_nbkt_key(const key_type& __key) const {
  return (_hash(__key) * 37ul + 7ul) % _num_buckets;
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::size_type
HashTable<_K, _V, _H>::_nbkt(const value_type& __obj) const {
  return _nbkt_key(__obj.first);
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::size_type
HashTable<_K, _V, _H>::_nbkt(const value_type& __obj, size_t __n) const {
  return (_hash(__obj.first) * 37ul + 7ul) % __n;
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::_Node*
HashTable<_K, _V, _H>::_new_node(const value_type& __obj) {
  _Node* n = _get_node();
  n->_next = NULL;
  try {
	_get_allocator().construct(&n->_val, __obj);
	return n;
  } catch(...) {
	_put_node(n);
	throw;
  }
}

template <typename _K, typename _V, typename _H>
void
HashTable<_K, _V, _H>::_delete_node(_Node* __n) {
  _get_allocator().destroy(&__n->_val);
  _put_node(__n);
}

template <typename _K, typename _V, typename _H>
HashTable<_K, _V, _H>::HashTable(size_type __n) {
  const size_type __n_buckets = _next_size(__n);
  _buckets.reserve(__n_buckets);
  _buckets.insert(_buckets.end(), __n_buckets, NULL);
  _num_elements = 0;
}

template <typename _K, typename _V, typename _H>
HashTable<_K, _V, _H>::~HashTable() {
  clear();
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::size_type
HashTable<_K, _V, _H>::size() const {
  return _num_elements;
}

template <typename _K, typename _V, typename _H>
bool
HashTable<_K, _V, _H>::empty() const {
  return 0 == size();
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::iterator
HashTable<_K, _V, _H>::begin() {
  for (size_type n=0; n<_num_buckets; ++n)
	if (NULL != _buckets[n])
	  return iterator(_buckets[n], this);
  return end();
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::iterator
HashTable<_K, _V, _H>::end() {
  return iterator(NULL, this);
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::iterator
HashTable<_K, _V, _H>::find(const key_type& __key) {
  size_type n = _nbkt_key(__key);
  _Node* p;
  for (p = _buckets[n];
	   (NULL != p) && (p->_val.first != __key);
	   p = p->_next);
  return iterator(p, this);
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::size_type
HashTable<_K, _V, _H>::count(const key_type& __key) const {
  size_type n = _nbkt_key(__key);
  _Node* p = _buckets[n];
  
  for (; NULL != p; p = p->_next)
	if (p->_val.first == __key)
	  return 1;
  
  return 0;
}

template <typename _K, typename _V, typename _H>
std::pair<typename HashTable<_K, _V, _H>::iterator, bool>
HashTable<_K, _V, _H>::insert(const value_type& __obj) {
  resize(_num_elements + 1);
  
  size_type n = _nbkt(__obj);
  _Node* p = _buckets[n];
      
  for (_Node* cur = p; NULL != cur; cur = cur->_next)
	if (cur->_val.first == __obj.first)
	  return std::make_pair(iterator(cur, this), false);
      
  _Node* tmp = _new_node(__obj);
  tmp->_next = p;
  _buckets[n] = tmp;
  ++ _num_elements;
  return std::make_pair(iterator(tmp, this), true);
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::reference
HashTable<_K, _V, _H>::find_or_insert(const value_type& __obj) {
  resize(_num_elements + 1);

  size_type n = _nbkt(__obj);
  _Node* p = _buckets[n];
      
  for (_Node* cur = p; NULL != cur; cur = cur->_next)
	if (cur->_val.first == __obj.first)
	  return cur->_val;
      
  _Node* tmp = _new_node(__obj);
  tmp->_next = p;
  _buckets[n] = tmp;
  ++ _num_elements;
  return tmp->_val;
}

template <typename _K, typename _V, typename _H>
typename HashTable<_K, _V, _H>::size_type
HashTable<_K, _V, _H>::erase(const key_type& __key) {
  size_type n = _nbkt_key(__key);
  _Node* p = _buckets[n];
      
  if (NULL != p) {
	_Node* cur = p;
	_Node* next = cur->_next;

	if (p->_val.first == __key) {
	  _buckets[n] = next;
	  _delete_node(p);
	  -- _num_elements;
	  return 1;
	}
	
	while (NULL != next) {
	  if (next->_val.first == __key) {
		cur->_next = next->_next;
		_delete_node(next);
		-- _num_elements;
		return 1;
	  } else {
		cur = next;
		next = cur->_next;
	  }
	}
  }
  
  return 0;
}

template <typename _K, typename _V, typename _H>
void
HashTable<_K, _V, _H>::resize(size_type __at_least) {
  size_type old_n = _num_buckets;
  
  if (__at_least > old_n)	{
	size_type n = _next_size(__at_least);
	
	if (n > old_n) {
	  vector_type v(n, NULL, _buckets.get_allocator());
	  try {
		_num_buckets = n;
		for (size_type i = 0; i < old_n; ++i) {
		  _Node* p = _buckets[i];
		  while (NULL != p)	{
			size_type j = _nbkt(p->_val, n);
			_buckets[i] = p->_next;
			p->_next = v[j];
			v[j] = p;
			p = _buckets[i];
		  }
		}
		_buckets.swap(v);
	  } catch(...) {
		for (size_type i = 0; i < v.size(); ++i) {
		  while (NULL != v[i]) {
			_Node* next = v[i]->_next;
			_delete_node(v[i]);
			v[i] = next;
		  }
		}
		throw;
	  }
	}
  }
}

template <typename _K, typename _V, typename _H>
void
HashTable<_K, _V, _H>::clear() {
  for (size_type i = 0; i < _num_buckets; ++i) {
	_Node* cur = _buckets[i];
	while (NULL != cur) {
	  _Node* next = cur->_next;
	  _delete_node(cur);
	  cur = next;
	}
	_buckets[i] = NULL;
  }
  _num_elements = 0;
}

