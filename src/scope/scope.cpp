/*****************************************************
 *  Implementation of the "Scope" abstract class.
 *
 *  This file also contains the implementation of
 *  "SocopeIterator".
 *
 *  Keltin Leung 
 */

#include "scope/scope.hpp"
#include "config.hpp"
#include "location.hpp"
#include "symb/symbol.hpp"

using namespace mind;
using namespace mind::symb;
using namespace mind::scope;

/*  Constructor (only for Scope).
 *
 *  PARAMETERS:
 *    start - the start position of the underlying iterator
 *    end   - the end position of the underlying iterator
 */
ScopeIterator::ScopeIterator(const miterator &start, const miterator &end)
    : _mit(start), _mit_end(end) {
    while (_mit != _mit_end && NULL == _mit->second)
        _mit++;
}

/*  Copy Constructor.
 *
 *  PARAMETERS:
 *    __x   - the reference interator
 */
ScopeIterator::ScopeIterator(const ScopeIterator &__x)
    : _mit(__x._mit), _mit_end(__x._mit_end) {}

/*  Determines whether two iterators are equivalent.
 *
 *  PARAMETERS:
 *    __x   - the reference iterator
 *  RETURNS:
 *    true if equivalent; false otherwise
 */
bool ScopeIterator::operator==(const ScopeIterator &__x) const {
    return (__x._mit == _mit);
}

/*  Determines whether two iterators are not equivalent.
 *
 *  PARAMETERS:
 *    __x   - the reference iterator
 *  RETURNS:
 *    false if equivalent; true otherwise
 */
bool ScopeIterator::operator!=(const ScopeIterator &__x) const {
    return (__x._mit != _mit);
}

/*  Advances this iterator.
 *
 *  RETURNS:
 *    the advanced version
 */
ScopeIterator &ScopeIterator::operator++() {
    do
        _mit++;
    while (_mit != _mit_end && NULL == _mit->second);
    return (*this);
}

/*  Advanced the iterator (and return the original version).
 *
 *  RETURNS:
 *    the original iterator
 */
ScopeIterator ScopeIterator::operator++(int) {
    ScopeIterator _tmp = *this;
    do
        _mit++;
    while (_mit != _mit_end && NULL == _mit->second);
    return _tmp;
}

/*  Gets the data pointed to by this iterator.
 *
 *  RETURNS:
 *    the data
 */
Symbol *ScopeIterator::operator*() const { return _mit->second; }

/*************************** Scope *****************************/

/*  Whether this scope is a GlobalScope.
 *
 *  RETURNS:
 *    true if it is a GlobalScope; false otherwise
 */
bool Scope::isGlobalScope(void) { return (getKind() == GLOBAL); }

/*  Whether this scope is a FuncScope.
 *
 *  RETURNS:
 *    true if it is a FuncScope; false otherwise
 */
bool Scope::isFuncScope(void) { return (getKind() == FUNCTION); }

/*  Looks up a name in this scope.
 *
 *  PARAMETERS:
 *    id    - the name
 *  RETURNS:
 *    the corresponding symbol if the name is defined; NULL otherwise
 */
Symbol *Scope::lookup(std::string id, Location *loc) {
    if (id.empty())
        return NULL;
    if (!_syms.count(id))
        return NULL;
    if (*loc < *_syms[id]->getDefLocation())
        return NULL;
    return _syms[id];
}

/*  Declares a symbol in this scope.
 *
 *  PARAMETERS:
 *    sym   - the symbol
 */
void Scope::declare(symb::Symbol *sym) {
    mind_assert(NULL != sym);

    sym->setScope(this);
    _syms[sym->getName()] = sym;
}

/*  Cancels a symbol that is declared in this scope.
 *
 *  PARAMETERS:
 *    sym   - the symbol to be cancelled
 */
void Scope::cancel(symb::Symbol *sym) {
    mind_assert(NULL != sym);

    if (sym->getScope() == NULL || sym->getScope() != this)
        return;
    else
        _syms[sym->getName()] = NULL;

    // just do our own things - don't bother other scopes
}

/*  Gets the iterator pointing to the start position of this scope.
 *
 *  RETURNS:
 *    the start iterator
 */
ScopeIterator Scope::begin(void) {
    ScopeIterator it(_syms.begin(), _syms.end());
    return it;
}

/*  Gets the end iterator pointing beyond the last position of this scope.
 *
 *  NOTE:
 *    the returned iterator does not point to the last element
 *  RETURNS:
 *    the end iterator
 */
ScopeIterator Scope::end(void) {
    ScopeIterator it(_syms.end(), _syms.end());
    return it;
}

/*  Prints the scope content.
 *
 *  PARAMETER:
 *    os    - the output stream
 *  RETURNS:
 *    the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Scope *s) {
    s->dump(os);
    return os;
}
