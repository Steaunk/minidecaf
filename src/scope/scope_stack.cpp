/*****************************************************
 *  Implementation of "ScopeStack".
 *
 *  Keltin Leung 
 */

#include "scope/scope_stack.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::scope;
using namespace mind::symb;
using namespace mind::util;

// the global ScopeStack instance
static ScopeStack __global_scope_stack;
ScopeStack *mind::scopes = &__global_scope_stack;

typedef Stack<Scope *> stk_t;

/*  Constructor.
 *
 */
ScopeStack::ScopeStack() { _global = NULL; }

/*  Looks up a name in the scope stack.
 *
 *  PARAMETERS:
 *    id      - the name to look up
 *    through - if set, we will look up the name in all visible scopes
 *              if not set, we just look up the name in the current scope
 *  RETURNS:
 *    the symbol if that name is defined; NULL other wise
 */
Symbol *ScopeStack::lookup(std::string id, Location *loc, bool through) {
    Symbol *s = NULL;

    if (id.empty())
        return NULL;

    if (through) {
        for (stk_t::iterator it = _stack.begin(); it != _stack.end(); ++it) {
            s = (*it)->lookup(id, loc);
            if (NULL != s)
                return s;
        }
        return NULL;

    } else {
        return _stack.top()->lookup(id, loc);
    }
}

/*  Declares a symbol in the current scope.
 *
 *  PARAMETERS:
 *    s     - the symbol
 */
void ScopeStack::declare(Symbol *s) {
    mind_assert(NULL != s && !_stack.empty());

    _stack.top()->declare(s);
}

/*  Opens a scope.
 *
 *  PARAMETERS:
 *    sco   - the scope to open
 */
void ScopeStack::open(Scope *sco) {
    mind_assert(NULL != sco);

    switch (sco->getKind()) {
    case Scope::GLOBAL:
        mind_assert(_stack.empty());
        _global = sco;
        break;
    case Scope::FUNCTION:
        // mind_assert(! _stack.empty());
        break;
    default:
        break;
    }

    _stack.push(sco);
}

/*  Closes the current scope.
 *
 */
void ScopeStack::close(void) {
    mind_assert(!_stack.empty());

    _stack.top() = NULL; // for garbage-collection
    _stack.pop();
}

/*  Gets the current scope.
 *
 *  RETURNS:
 *    the current scope
 */
Scope *ScopeStack::top(void) {
    mind_assert(!_stack.empty());

    return _stack.top();
}

/*  Looks up the topmost scope of a specific kind.
 *
 *  PARAMETERS:
 *    kind  - the specific kind
 *  RETURNS:
 *    the topmost scope if found; NULL otherwise
 */
Scope *ScopeStack::lookForScope(Scope::kind_t kind) {
    for (stk_t::iterator it = _stack.begin(); it != _stack.end(); ++it)
        if ((*it)->getKind() == kind)
            return (*it);

    return NULL;
}
