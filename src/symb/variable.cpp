/*****************************************************
 *  Implementation of "Variable".
 *
 *  Keltin Leung 
 */

#include "config.hpp"
#include "scope/scope.hpp"
#include "symb/symbol.hpp"

using namespace mind;
using namespace mind::symb;
using namespace mind::type;
using namespace mind::scope;
using namespace mind::tac;

/* Constructor.
 *
 * PARAMETERS:
 *   n     - the variable name
 *   t     - the type
 *   l     - the definition location in the source code
 */
Variable::Variable(std::string n, Type *t, Location *l) {
    mind_assert(NULL != t);

    name = n;
    type = t;
    loc = l;
    order = -1;

    is_parameter = false;
    global_init = 0;
    attached = NULL;

    mark = 0;
}
Variable::Variable(std::string n, Type *t, ast::DimList *dim, Location *l) {
    mind_assert(NULL != t);

    name = n;
    type = t;
    loc = l;
    order = -1;

    is_parameter = false;
    global_init = 0;
    attached = NULL;

    this->dim = dim;

    mark = 0;
}

/* Sets the parameter flag (i.e. it is a parameter).
 *
 */
void Variable::setParameter(void) { is_parameter = true; }

/* Tests whether it is a parameter.
 *
 * RETURNS:
 *   true if it is a parameter; false otherwise
 */
bool Variable::isParameter(void) { return is_parameter; }

void Variable::setGlobalInit(int val) { global_init = val; }

int Variable::getGlobalInit() { return global_init; }

/* Tests whether it is a local variable.
 *
 * RETURNS:
 *   true if it is a local variable, false otherwise
 */
bool Variable::isLocalVar(void) {
    mind_assert(NULL != defined_in);

    return (defined_in->isFuncScope() && !is_parameter);
}

/* Tests whether it is a global variable.
 *
 * RETURNS:
 *   true if it is a global variable, false otherwise
 */
bool Variable::isGlobalVar(void) {
    mind_assert(NULL != defined_in);

    return defined_in->isGlobalScope();
}

/* Tests whether this symbol is a Variable.
 *
 * RETURNS:
 *   true
 */
bool Variable::isVariable(void) { return true; }

/* Prints this symbol.
 *
 * PARAMETERS:
 *   os    - the output stream
 */
void Variable::dump(std::ostream &os) {
    os << loc << " -> variable ";
    if (is_parameter)
        os << "@" << (name);
    else
        os << (name);
    os << " : " << type;
    if (isGlobalVar())
        os << " = " << global_init;
}

/* Attaches the register object to this symbol.
 *
 * PARAMETERS:
 *   v     - the register object
 */
void Variable::attachTemp(tac::Temp v) {
    mind_assert(NULL != v && NULL == attached);

    attached = v;
}

/* Gets the attached register object.
 *
 * RETURNS:
 *   the attached register object
 */
Temp Variable::getTemp(void) { return attached; }

ast::DimList *Variable::getDimList(void) { return dim; }
