/*****************************************************
 *  Implementation of "Function".
 *
 *  Keltin Leung 
 */

#include "config.hpp"
#include "scope/scope.hpp"
#include "scope/scope_stack.hpp"
#include "symb/symbol.hpp"
#include "tac/tac.hpp"
#include "type/type.hpp"

using namespace mind;
using namespace mind::symb;
using namespace mind::type;
using namespace mind::scope;
using namespace mind::tac;

/* Constructor.
 *
 * NOTE:
 *   the FuncType will be automatically created
 * PARAMETERS:
 *   n       - the function name
 *   resType - the result type
 *   l       - the definition location in the source code
 */
Function::Function(std::string n, Type *resType, Location *l) {
    mind_assert(NULL != resType);

    name = n;
    loc = l;
    order = -1;
    mark = 0;

    type = new FuncType(resType);
    associated = new FuncScope(this);
    attached = NULL;
    entry = NULL;
}

/* Gets the associated function scope.
 *
 * RETURNS:
 *   the associated function scope
 */
FuncScope *Function::getAssociatedScope(void) { return associated; }

/* Gets the result type.
 *
 * RETURNS:
 *   the result type
 */
Type *Function::getResultType(void) { return getType()->getResultType(); }

/* Gets the scope where this symbol is defined.
 *
 * RETURNS:
 *   the scope where this symbol is defined
 */
Scope *Function::getScope(void) { return defined_in; }

/* Gets the type of this symbol.
 *
 * NOTE:
 *   it is different from the GetResultType()
 * RETURNS:
 *   the corresponding FuncType
 */
FuncType *Function::getType(void) { return (FuncType *)type; }

/* Appends a parameter to this function.
 *
 * NOTE:
 *   you should check the correctness of the argument before invoking me
 * PARAMETERS:
 *   arg   - the argument
 */
void Function::appendParameter(Variable *arg) {
    mind_assert(NULL != arg);

    // it is your responsibility to check "arg" before invoking this method
    arg->setParameter();
    arg->setOrder(getType()->numOfParameters());
    getType()->appendParameter(arg->getType());
    // usually the symbol has already been added into the associated scope,
    // we just make sure it is right (and we will ignore the duplicated
    // declarations)
    // if (associated->lookup(arg->getName()) == NULL)
    // associated->declare(arg);
}

/* Tests whether this symbol is a Function.
 *
 * RETURNS:
 *   true
 */
bool Function::isFunction(void) { return true; }

/* Prints this symbol.
 *
 * PARAMETERS:
 *   os    - the output stream
 */
void Function::dump(std::ostream &os) {
    os << loc << " -> function " << (name) << " : " << type;
}

/* Attaches the entry label to this function symbol.
 *
 * PARAMETERS:
 *   l     - the label
 */
void Function::attachEntryLabel(Label l) {
    mind_assert(NULL != l && NULL == entry);

    entry = l;
}

/* Attaches the Functy object to this function symbol.
 *
 * PARAMETERS:
 *   f     - the Functy object
 */
void Function::attachFuncty(Functy f) {
    mind_assert(NULL != f && NULL == attached);

    attached = f;
}

/* Gets the attached entry label.
 *
 * RETURNS:
 *   the attached entry label
 */
Label Function::getEntryLabel(void) { return entry; }

/* Gets the attached Functy object.
 *
 * RETURNS:
 *   the attached Functy object
 */
Functy Function::getFuncty(void) { return attached; }
