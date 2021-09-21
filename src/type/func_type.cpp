/*****************************************************
 *  Implementation of the "FuncType" class.
 *
 *  Keltin Leung 
 */

#include "config.hpp"
#include "type/type.hpp"

using namespace mind::type;
using namespace mind::util;

typedef List<Type *> TypeList;

/* Constructor.
 *
 * PARAMETERS:
 *   result - the result type
 */
FuncType::FuncType(Type *result) {
    result_type = result;
    arglist = new TypeList();
}

/* Tests whether this type is FuncType.
 *
 * RETURNS:
 *   true
 */
bool FuncType::isFuncType(void) { return true; }

/* Gets the result type.
 *
 * RETURNS:
 *   the result type
 */
Type *FuncType::getResultType(void) { return result_type; }

/* Gets the argument type list.
 *
 * RETURNS:
 *   the argument type list
 */
List<Type *> *FuncType::getArgList(void) { return arglist; }

/* Gets the number of arguments.
 *
 * RETURNS:
 *   the number of arguments
 */
size_t FuncType::numOfParameters(void) { return arglist->length(); }

/* Appends an argument to the end of the argument list.
 *
 * PARAMETERS:
 *   t     - the type of that argument
 */
void FuncType::appendParameter(Type *t) { arglist->append(t); }

/* Tests whether this type is compatible with the given type.
 *
 * NOTE:
 *   this function is used to determine the overriding relationship of functions
 * PARAMETERS:
 *   t     - the given type
 * RETURNS:
 *   true if this type is compatible with the given type; false otherwise
 */
bool FuncType::compatible(Type *t) {
    mind_assert(NULL != t);

    if (!t->isFuncType())
        return false;
    else {
        FuncType *ft = (FuncType *)t;
        if (!result_type->compatible(ft->result_type) ||
            arglist->length() != ft->arglist->length())
            return false; // result types and arglist lengths must match

        TypeList::iterator it1, it2;
        it1 = arglist->begin();
        it2 = ft->arglist->begin();

        if (!(*it1)->compatible(*it2))
            return false; // "this" types must match

        int n = arglist->length();
        for (int i = 1; i < n; ++i) {
            ++it1;
            ++it2;
            if (!(*it2)->compatible(*it1))
                return false; // every pair of parameter types must match
        }

        return true;
    }
}

/* Tests whether this type is equal to the given type.
 *
 * NOTE:
 *   you should never use this function (and it will not return)
 * PARAMETERS:
 *   t     - the given type
 * RETURNS:
 *   false
 */
bool FuncType::equal(Type *t) {
    mind_assert(false); // you should never compare two function types..

    return false;
}

/* Prints this type.
 *
 * PARAMETERS:
 *   os    - the output stream
 */
void FuncType::dump(std::ostream &os) {
    TypeList::iterator it;
    for (it = arglist->begin(); it != arglist->end(); ++it)
        os << (*it) << "->";
    os << result_type;
}
