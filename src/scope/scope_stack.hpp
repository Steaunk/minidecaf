/*****************************************************
 *  Definition of ScopeStack.
 *
 *  We organize all the open scopes by a stack, at top
 *  of which is the innermost scope. GlobalScope is at
 *  the bottom of the stack.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_SCOPESTACK__
#define __MIND_SCOPESTACK__

#include "3rdparty/stack.hpp"
#include "define.hpp"
#include "scope/scope.hpp"

namespace mind {

#define MIND_SCOPESTACK_DEFIINED
namespace scope {

/* Scope Stack.
 *
 * We orgainize all the visible scopes with a stack, the topmost of which
 * is the innermost open scope.
 */
class ScopeStack {
  private:
    // the underlying stack
    util::Stack<Scope *> _stack;
    // a track of the global scope
    Scope *_global;

  public:
    // Constructor
    ScopeStack();
    // Looks up a name in the scope stack
    symb::Symbol *lookup(std::string id, Location *loc, bool through = true);
    // Declares a symbol in the current scope
    void declare(symb::Symbol *s);
    // Opens a scope
    void open(Scope *s);
    // Closes the current scope
    void close(void);
    // Gets the current scope
    Scope *top(void);
    // Looks up the topmost scope of a specific kind
    Scope *lookForScope(Scope::kind_t kind);
};
} // namespace scope
} // namespace mind

#endif // __MIND_SCOPESTACK__
