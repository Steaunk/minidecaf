/*****************************************************
 *  Implementation of "FuncScope".
 *
 *  Keltin Leung 
 */

#include "3rdparty/vector.hpp"
#include "config.hpp"
#include "scope/scope.hpp"
#include "symb/symbol.hpp"

#include <algorithm>

using namespace mind;
using namespace mind::symb;
using namespace mind::scope;
using namespace mind::util;

/*  Constructor.
 *
 *  PARAMETERS:
 *    o     - the owner of this function scope
 */
FuncScope::FuncScope(Function *o) {
    mind_assert(NULL != o);

    owner = o;
}

/*  Gets the owner of this scope.
 *
 *  RETURNS:
 *    the owner of this function scope
 */
Function *FuncScope::getOwner(void) { return owner; }

/*  Gets the kind of this scope.
 *
 *  RETURNS:
 *    Scope::FUNCTION
 */
Scope::kind_t FuncScope::getKind(void) { return FUNCTION; }

/*  Declares a symbol in this scope.
 *
 *  PARAMETERS:
 *    s     - the symbol
 */
void FuncScope::declare(Symbol *s) {
    mind_assert(NULL != s && s->isVariable() && s->getScope() == NULL);

    Scope::declare(s);
}

// symbol comparator
struct symbol_less {
    bool operator()(Symbol *s1, Symbol *s2) {
        return (std::strcmp((s1->getName().c_str()), (s2->getName().c_str())) <
                0);
    }
};

/*  Prints the content of this scope.
 *
 *  PARAMETERS:
 *    os    - the output stream
 */
void FuncScope::dump(std::ostream &os) {
    Vector<Symbol *> s;
    for (ScopeIterator it = begin(); it != end(); ++it)
        s.push_back(*it);
    std::sort(s.begin(), s.end(), symbol_less());

    os << "FUNCTION SCOPE OF '" << (owner->getName()) << "':";
    incIndent(os);
    for (Vector<Symbol *>::iterator it = s.begin(); it != s.end(); ++it) {
        newLine(os);
        os << "  " << *it;
    }
    decIndent(os);
}
