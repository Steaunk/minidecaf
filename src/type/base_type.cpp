/*****************************************************
 *  Implementation of the "BaseType" class.
 *
 *  Keltin Leung 
 */

#include "config.hpp"
#include "type/type.hpp"

using namespace mind::type;

#define WORD_SIZE 4

#define EITHER_ERR(a, b) ((a == BaseType::Error) || (b == BaseType::Error))

/* Constructor.
 *
 * PARAMETERS:
 *   str   - the name of this base type
 */
BaseType::BaseType(const char *str) {
    mind_assert(NULL != str);

    type_name = str;
}

/* Get the size of this type
 */
int BaseType::getSize() { return WORD_SIZE; }

/* Tests whether this type is BaseType.
 *
 * RETURNS:
 *   true
 */
bool BaseType::isBaseType(void) { return true; }

/* Tests whether this type is compatible with the given type.
 *
 * PARAMETERS:
 *   t     - the given type
 * RETURNS:
 *   true if this type is compatible with the given type; false otherwise
 */
bool BaseType::compatible(Type *t) {
    mind_assert(NULL != t);

    return t->isBaseType();
}

/* Tests whether this type is equal to the given type.
 *
 * PARAMETERS:
 *   t     - the given type
 * RETURNS:
 *   true if this type is equal to the given type; false otherwise
 */
bool BaseType::equal(Type *t) {
    mind_assert(NULL != t);

    return (this == t);
}

/* Prints this type.
 *
 * PARAMETERS:
 *   os    - the output stream
 */
void BaseType::dump(std::ostream &os) { os << type_name; }

// private instances of the intrinsic types
BaseType BaseType::_int("Int");
BaseType BaseType::_error("Error");

// public interfaces (constant)
BaseType *const BaseType::Int = &_int;
BaseType *const BaseType::Error = &_error;
