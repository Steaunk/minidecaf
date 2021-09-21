/*****************************************************
 *  Implementation of "LocalScope".
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

/*  Gets the kind of this scope.
 *
 *  RETURNS:
 *    Scope::LOCAL
 */
Scope::kind_t LocalScope::getKind(void) { return LOCAL; }

/*  Declares a symbol in this scope.
 *
 *  PARAMETERS:
 *    s     - the symbol
 */
void LocalScope::declare(Symbol *s) {
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
void LocalScope::dump(std::ostream &os) {
    Vector<Symbol *> s;
    for (ScopeIterator it = begin(); it != end(); ++it)
        s.push_back(*it);
    std::sort(s.begin(), s.end(), symbol_less());

    os << "LOCAL SCOPE:";
    incIndent(os);
    for (Vector<Symbol *>::iterator it = s.begin(); it != s.end(); ++it) {
        newLine(os);
        os << "  " << *it;
    }
    decIndent(os);
}
