/*****************************************************
 *  Hash Functions, HashTable and HashMap.
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

#ifndef __MIND_HASH__
#define __MIND_HASH__

#include "boehmgc.hpp"

#include <cstddef>
#include <utility>
#include <vector>
#include <algorithm>

using std::size_t;

namespace mind {

  namespace util {

	/* A set of hash functions.
	 *
	 * Hash functions for:
	 *   const char*
	 *   int & unsigned int
	 *   long & unsigned long
	 *   void* (for pointers)
	 */
	template<class _K>
	struct hash { };

	// implementation of the hash functions
#include "hash_fun.hpp"

	// Hash Table Node (don't use it explicitly)
	template <typename _V>
	struct _HashNode {
	  _HashNode* _next;
	  _V         _val;
	};

	// forward declaration of HashTable
	template <typename _K, typename _V, typename _H>
	class HashTable;

	/* Hash Table Iterator.
	 *
	 * Don't use it explicitly. Use XXXX::iterator instead.
	 *
	 * TEMPLATE ARGUMENTS:
	 *   _K  - the key type
	 *   _V  - the value type
	 *   _H  - the hash function type
	 * PUBLIC INTERFACES:
	 *   reference  operator * ()
	 *       - Gets the reference of the element pointed by this iterator
	 *       NOTE: the return value is a <key, value> pair
	 *   pointer    operator -> ()
	 *       - Gets the pointer of the element pointed by this iterator
	 *       NOTE: the return value is a <key, value> pair
	 *   iterator&  operator ++ ()
	 *       - Advances this iterator and returns the new iterator
	 *   iterator   operator ++ (int)
	 *       - Advances this iterator and returns the old iterator
	 *   bool       operator == (const iterator& i)
	 *       - Tests whether this iterator equals another iterator
	 *   bool       operator != (const iterator& i)
	 *       - Tests whether this iterator is not equal to another one
	 */
	template <typename _K, typename _V, typename _H>
	struct _HashIterator {
	  // the value type
	  typedef std::pair<_K, _V>          value_type;
	  // the HashTable type
	  typedef HashTable<_K, _V, _H>      _HashTable;
	  // the HashNode type
	  typedef _HashNode<value_type>      _Node;
	  // the self type
	  typedef _HashIterator<_K, _V, _H>  iterator;
	  // the size type
	  typedef size_t                     size_type;
	  // the reference type
	  typedef value_type&                reference;
	  // the pointer type
	  typedef value_type*                pointer;

	  // the current node
	  _Node*        _cur;
	  // the underlying hash table
	  _HashTable*   _ht;

	  // a private constructor for HashTable
	  _HashIterator(_Node*, _HashTable*);
	  // the empty constructor
	  _HashIterator();
	  // Dereference operator
	  reference  operator * () const;
	  // Pointer operator
	  pointer    operator -> () const;
	  // Prefix incremental operator
	  iterator&  operator ++ ();
	  // Postfix incremental operator
	  iterator   operator ++ (int);
	  // Testing equivalence
	  bool       operator == (const iterator&) const;
	  // Testing unequivalence
	  bool       operator != (const iterator&) const;
	};

	/* Hash Table.
	 *
	 * We sugguest you use HashMap or HashSet instead of using this class directly.
	 *
	 * TEMPLATE ARGUMENTS:
	 *   _K  - the key type
	 *   _V  - the value type
	 *   _H  - the hash function type
	 * PUBLIC INTERFACES:
	 *   iterator
	 *       - the iterator type
	 *
	 *   HashTable(size_type n)
	 *       - Constructs an instance of HashTable
	 *       PARAMETERS:
	 *         n     - the reserved capacity
	 *
	 *   size_type                  size(void)
	 *       - Gets the number of elements in this hash table
	 *
	 *   void                       resize(size_type n)
	 *       - Resizes the hash table
	 *       PARAMETERS:
	 *         n     - the number of elements to be held
	 *
	 *   bool                       empty(void)
	 *       - Tests whether this hash table is empty
	 *
	 *   size_type                  erase(const key_type& k)
	 *       - Erases the node with the specified key
	 *       PARAMETERS:
	 *         k     - the element key
	 *
	 *   void                       clear(void)
	 *       - Clears the whole hash table
	 *
	 *   iterator                   begin(void)
	 *       - Gets an iterator pointing to the first element
	 *
	 *   iterator                   end(void)
	 *       - Gets an iterator pointing beyond the last element
	 *
	 *   std::pair<iterator, bool>  insert(const value_type& v)
	 *       - Inserts a <key, value> pair into the hash table
	 *       PARAMETERS:
	 *         v     - the <key, value> pair
	 *       RETURNS:
	 *         if succeeded, the iterator pointing to the new pair
	 *         and a Boolean flag of "true" will be returned;
	 *         otherwise, the iterator pointing to an existing node
	 *         with the same key and a Boolean flag of "false" will
	 *         be returned.
	 *
	 *   reference                  find_or_insert(const value_type& v)
	 *       - Finds a node with the specified key or inserts the specified
	 *         <key, value> pair into the hash table
	 *       PARAMETERS:
	 *         v     - the <key, value> pair
	 *
	 *   iterator                   find(const key_type& k)
	 *       - Finds a node with the specified key
	 *       PARAMETERS:
	 *         k     - the specified key
	 *       RETURNS:
	 *         an iterator pointing to the target node if found; end() otherwise.
	 *
	 *   size_type                  count(const key_type& k)
	 *       - Counts the nodes with the specified key
	 *       PARAMETERS:
	 *         k     - the specified key
	 *       RETURNS:
	 *         1 if a node with the specified key exists; 0 otherwise.
	 *       NOTE:
	 *         there cannot be multiple nodes with the same key in the hash table
	 */
	template <typename _K, typename _V, typename _H = hash<_K> >
	class HashTable {
	public:
	  // the hash function type
	  typedef _H                        hasher;
	  // the key type
	  typedef _K                        key_type;
	  // the node value type
	  typedef std::pair<_K, _V>         value_type;
	  // the size type
	  typedef size_t                    size_type;
	  // the reference type
	  typedef value_type&               reference;
	  // the iterator type
	  typedef _HashIterator<_K, _V, _H> iterator;

	  friend struct _HashIterator<_K, _V, _H>;

	  // Constructor.
	  HashTable(size_type);
	  // Gets the number of nodes.
	  size_type                  size(void) const;
	  // Tests whether it is empty.
	  bool                       empty(void) const;
	  // Gets the iterator pointing to the first element.
	  iterator                   begin(void);
	  // Gets the iterator pointing beyond the last element.
	  iterator                   end(void);
	  // Inserts a <key, value> pair.
	  std::pair<iterator, bool>  insert(const value_type&);
	  // Finds or insert-if-not-founds a <key, value> pair.
	  reference                  find_or_insert(const value_type&);
	  // Finds the node with the specified key.
	  iterator                   find(const key_type&);
	  // Counts the nodes with the specified key.
	  size_type                  count(const key_type&) const;
	  // Erases the nodes with the specified key.
	  size_type                  erase(const key_type&);
	  // Resizes the hash table.
	  void                       resize(size_type);
	  // Deletes all nodes in the hash table
	  void                       clear(void);
	  // Destructor.
	  ~HashTable();

	private:
	  typedef _HashNode<value_type>        _Node;
	  typedef gc_allocator<value_type>     _Alloc;
      typedef typename _Alloc::template rebind<_Node>::other
	  _Node_Alloc;
      typedef typename _Alloc::template rebind<_Node*>::other
	  _Nodeptr_Alloc;
      typedef typename _Alloc::template rebind<value_type>::other
	  allocator_type;
	  typedef std::vector<_Node*, _Nodeptr_Alloc>
	  vector_type;

	  hasher                _hash;
	  size_type             _num_buckets;
	  size_type             _num_elements;
	  vector_type           _buckets;
	  _Node_Alloc           _node_allocator;
      
      allocator_type        _get_allocator(void) const;
	  _Node*                _get_node(void);
	  void                  _put_node(_Node*);
	  size_type             _next_size(size_type) const;
	  size_type             _nbkt_key(const key_type&) const;
	  size_type             _nbkt(const value_type&) const;
	  size_type             _nbkt(const value_type&, size_t) const;
	  _Node*                _new_node(const value_type&);
	  void                  _delete_node(_Node*);
	};

	// implementation of the hash table iterator
#include "hash_iterator.hpp"
	// implementation of the hash table
#include "hash_table.hpp"

	/* Hashed Map.
	 *
	 * TEMPLATE ARGUMENTS:
	 *   _K  - the key type
	 *   _V  - the value type
	 *   _H  - the hash function type
	 * PUBLIC INTERFACES:
	 *   iterator
	 *       - the iterator type
	 *
	 *   HashMap()
	 *       - Constructs a hashed map.
	 *   HashMap(size_type n)
	 *       - Constructs an instance of HashMap with the given size
	 *       PARAMETERS:
	 *         n     - the reserved capacity
	 *
	 *   size_type                  size(void)
	 *       - Gets the number of elements in this hashed map
	 *       NOTE: "number of elements" here means "number of keys",
	 *             even if the key maps to nothing
	 *
	 *   void                       resize(size_type n)
	 *       - Resizes the hashed map
	 *       PARAMETERS:
	 *         n     - the number of elements to be held
	 *
	 *   bool                       empty(void)
	 *       - Tests whether this hashed map is empty
	 *
	 *   size_type                  erase(const key_type& k)
	 *       - Erases the element with the specified key
	 *       PARAMETERS:
	 *         k     - the element key
	 *
	 *   void                       clear(void)
	 *       - Clears the whole hashed map
	 *
	 *   iterator                   begin(void)
	 *       - Gets an iterator pointing to the first element
	 *
	 *   iterator                   end(void)
	 *       - Gets an iterator pointing beyond the last element
	 *
	 *   std::pair<iterator, bool>  insert(const value_type& v)
	 *       - Inserts a <key, value> pair into the hashed map
	 *       PARAMETERS:
	 *         v     - the <key, value> pair
	 *       RETURNS:
	 *         if succeeded, the iterator pointing to the new pair
	 *         and a Boolean flag of "true" will be returned;
	 *         otherwise, the iterator pointing to an existing node
	 *         with the same key and a Boolean flag of "false" will
	 *         be returned.
	 *
	 *   _V&                        operator[](const key_type& k)
	 *       - Gets the reference to the value mapped by the given key
	 *       PARAMETERS:
	 *         k     - the key
	 *       RETURNS:
	 *         the reference to the value mapped by the given key.
	 *         if there is no node with the specified key in the map,
	 *         create one with the default constructor of the value type.
	 *
	 *   iterator                   find(const key_type& k)
	 *       - Finds a node with the specified key
	 *       PARAMETERS:
	 *         k     - the specified key
	 *       RETURNS:
	 *         an iterator pointing to the target node if found; end() otherwise.
	 *
	 *   size_type                  count(const key_type& k)
	 *       - Counts the nodes with the specified key
	 *       PARAMETERS:
	 *         k     - the specified key
	 *       RETURNS:
	 *         1 if a node with the specified key exists; 0 otherwise.
	 *       NOTE:
	 *         there cannot be multiple nodes with the same key in the hashed map
	 */
	template <typename _K, typename _V, typename _H = hash<_K> >
	class HashMap {
	private:
	  typedef HashTable<_K, _V, _H>  _HashTable;
	  _HashTable  _ht;

	public:
	  // the key type
	  typedef typename _HashTable::key_type   key_type;
	  // the node value type
	  typedef typename _HashTable::value_type value_type;
	  // the hash function type
	  typedef typename _HashTable::hasher     hasher;
      // the size type
	  typedef size_t                          size_type;
	  // the reference type
	  typedef typename _HashTable::reference  reference;
	  // the iterator type
	  typedef typename _HashTable::iterator   iterator;

	  // Constructor
	  HashMap();
	  // Constructor (with the capacity argument)
	  explicit HashMap(size_type);
	  // Gets the size of the map
	  size_type                 size(void) const;
	  // Resizes the map
	  void                      resize(size_type);
	  // Tests whether the map is empty
	  bool                      empty(void) const;
	  // Gets an iterator pointing to the first element
	  iterator                  begin(void);
	  // Gets an iterator pointing beyond the last element
	  iterator                  end(void);
	  // Inserts a <key, value> pair to the map
	  std::pair<iterator, bool> insert(const value_type&);
	  // Finds an element with the given key
	  iterator                  find(const key_type&);
	  // Gets the reference to the value mapped by the given key
	  _V&                       operator[](const key_type&);
	  // Counts the elements with the given key
	  size_type                 count(const key_type&) const;
	  // Erases the elements with the given key
	  size_type                 erase(const key_type&);
	  // Clears the map
	  void                      clear(void);
	};

	// implementation of the hashed map
#include "hash_map.hpp"
	
  }
}
	
#endif // _MIND_HASH__
