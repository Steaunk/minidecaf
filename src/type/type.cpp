/*****************************************************
 *  Implementation of the "Type" abstract class.
 *
 *  Keltin Leung 
 */

#include "type/type.hpp"
#include "config.hpp"

using namespace mind::type;

/* Tests whether this type is BaseType.
 *
 * RETURNS:
 *   true if this type is BaseType; false otherwise
 */
bool Type::isBaseType(void) { return false; }

/* Tests whether this type is ArrayType.
 *
 * RETURNS:
 *   true if this type is ArrayType; false otherwise
 */
bool Type::isArrayType(void) { return false; }

/* Tests whether this type is FuncType.
 *
 * RETURNS:
 *   true if this type is FuncType; false otherwise
 */
bool Type::isFuncType(void) { return false; }

/* Prints a Type object.
 *
 * PARAMETERS:
 *   os    - the output stream
 *   t     - the type object
 * RETURNS:
 *   the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, Type *t) {
    t->dump(os);
    return os;
}
