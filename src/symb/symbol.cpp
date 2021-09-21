/*****************************************************
 *  Implementation of the "Symbol" abstract class.
 *
 *  Keltin Leung 
 */

#include "symb/symbol.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::symb;
using namespace mind::type;
using namespace mind::scope;

/* Gets the name of this symbol.
 *
 * RETURNS:
 *   name of this symbol
 */
std::string Symbol::getName(void) { return name; }

/* Gets the type of this symbol.
 *
 * RETURNS:
 *   type of this symbol
 */
Type *Symbol::getType(void) { return type; }

/* Gets the definition location in the source code.
 *
 * RETURNS:
 *   the definition location
 */
Location *Symbol::getDefLocation(void) { return loc; }

/* Tests whether it is a Variable.
 *
 * RETURNS:
 *   true if yes, false otherwise
 */
bool Symbol::isVariable(void) { return false; }

/* Tests whether it is a Function.
 *
 * RETURNS:
 *   true if yes, false otherwise
 */
bool Symbol::isFunction(void) { return false; }

/* Gets the order of this symbol.
 *
 * RETURNS:
 *   the order of this symbol
 */
int Symbol::getOrder(void) { return order; }

/* Sets the order of this symbol.
 *
 * PARAMETERS:
 *   ord   - the order
 */
void Symbol::setOrder(int ord) {
    mind_assert(ord >= 0);

    order = ord;
}

/* Gets the scope where this symbol is defined.
 *
 * RETURNS:
 *   the scope where this symbol is defined
 */
scope::Scope *Symbol::getScope(void) { return defined_in; }

/* Sets the scope where this symbol is defined.
 *
 * PARAMETERS:
 *   s     - the scope where this symbol is defined
 */
void Symbol::setScope(scope::Scope *s) {
    mind_assert(NULL != s);

    defined_in = s;
}

/* Prints this symbol.
 *
 * PARAMETERS:
 *   os    - the output stream
 * RETURNS:
 *   the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Symbol *sym) {
    sym->dump(os);
    return os;
}
