/*  The error handler routines.
 *
 *  CHANGE LOG:
 *    2007-04-20: Change "std::cerr" in "issue()" and "checkPoint()" into
 * "std::cout". 2007-04-30: Add ErrorBuffer.
 *
 *
 *  Keltin Leung, 2007/04/20
 */

#include "config.hpp"
#include "errorbuf.hpp"
#include "location.hpp"
#include "scope/scope.hpp"
#include "symb/symbol.hpp"
#include "type/type.hpp"

#include <cstdarg>
#include <cstdio>
#include <sstream>

using namespace mind;
using namespace mind::symb;
using namespace mind::scope;
using namespace mind::type;
using namespace mind::err;

// number of the errors
static int num_of_errors = 0;

// buffer of the error messages
static ErrorBuffer ebuff(std::cerr);

/* Issues an error.
 *
 * PARAMETER:
 *   loc   - the location of the error
 *   err   - the error object to be issued
 */
void mind::err::issue(Location *loc, MindError *err) {
    std::ostringstream oss;

    oss << "*** Error at " << loc << ": ";
    err->printTo(oss);
    oss << ".";

    ebuff.add(loc, oss.str());

    ++num_of_errors;
}

/* Gets the number of errors.
 *
 * RETURNS:
 *   the number of errors had been issued so far
 */
int mind::err::numOfErrors(void) { return num_of_errors; }

/* Checks whether there has been no errors so far
 *
 * NOTE:
 *   if there are any errors, the compilation process will be terminated.
 */
void mind::err::debug(const char *msg, ...) {
    using namespace std;
    va_list ap;

    fprintf(stderr, " * Mind Debug >>> ");
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

/* Issues a bad assertion.
 *
 * PARAMETERS:
 *   msg   - the assertion message
 *   file  - the filename containing the assertion
 *   line  - the line number of the assertion
 * NOTE:
 *   do NOT call me directly. please use 'mind_assert(...)' instead.
 */
void mind::err::bad_assertion(const char *msg, const char *file, int line) {
    std::cerr << "*** Assertion '" << msg << "' at(" << file << ":" << line
              << ") failed!" << std::endl;
    std::cerr << "    Please check your code." << std::endl;
    std::exit(1);
    // don't use "std::abort()", because it is defined as "crash" and will lead
    // to core dump.
}

/* Prints some debug message.
 *
 * PARAMETERS:
 *   msg   - the format string
 * NOTE:
 *   the same usage with the "printf" function in standard C
 */
void mind::err::checkPoint(void) {
    if (num_of_errors > 0) {
        ebuff.flush();
        std::cerr << "Compilation process terminated due to previous "
                  << num_of_errors << " errors." << std::endl;
        exit(1);
    }
}

/***************** Details about each kind of error ******************/

/* Unrecognized Character Error.
 *
 * CONDITION:
 *   when the scanner meets an undefined character.
 * PARAMETERS:
 *   c     - the illegal character
 */
UnrecogCharError::UnrecogCharError(char c) { ch = c; }

// "unrecognized character '?'"
void UnrecogCharError::printTo(std::ostream &os) {
    os << "unrecognized character '" << ch << "'";
}

/* Integer Too Large Error.
 *
 * CONDITION:
 *   when the scanner gets an integer literal string whose value is larger than
 * LONG_MAX (SEE ALSO: std::strtol).
 */
IntTooLargeError::IntTooLargeError() {}

// "integer literal is too large"
void IntTooLargeError::printTo(std::ostream &os) {
    os << "integer literal is too large";
}
/* Comment not closed Error
 *
 * CONDITION:
 *   when the scanner gets a block comment starting delimiter without a closing
 * delimiter
 */
CommentNotClosedError::CommentNotClosedError() {}

// "integer literal is too large"
void CommentNotClosedError::printTo(std::ostream &os) {
    os << "block comment is not closed";
}
/* Syntax Error.
 *
 * CONDITION:
 *   when the parser gets a syntax error.
 * PARAMETERS:
 *   m     - the error message
 */
SyntaxError::SyntaxError(std::string m) { msg = m; }

// "syntax error"
void SyntaxError::printTo(std::ostream &os) { os << msg; }

/* Bad Argument Count Error.
 *
 * CONDITION:
 *   when the actual argument count doesn't agree with the formal argument list.
 * PARAMETERS:
 *   f     - the function involved
 */
BadArgCountError::BadArgCountError(Function *f) {
    fn_name = f->getName();
    arg_count = f->getType()->numOfParameters() - 1;
}

// "wrong number of actual parameters for 'foo': 3 expected"
void BadArgCountError::printTo(std::ostream &os) {
    os << "wrong number of actual parameters for '" << fn_name
       << "': " << arg_count << " expected";
}

/* Bad Print Statement Error.
 *
 * CONDITION:
 *   "print" statement only accepts expressions of Int or Bool.
 *   if the argument for print statement is not Int or Bool...
 */
BadPrintStmtError::BadPrintStmtError() {}

// "unexpected parameter type for 'print': 'Int' or 'Bool' expected"
void BadPrintStmtError::printTo(std::ostream &os) {
    os << "unexpected parameter type for 'print': 'Int' or 'Bool' expected";
}

/* Bad Receiver Error.
 *
 * CONDITION:
 *   when the receriver of a message (i.e. function call) is not an object
 *   i.e.:   calling XXX.foo() but XXX is not a class object
 *   note:   calling YYY.length() when YYY is an array is legal.
 * PARAMETERS:
 *   fun_name - name of the message (i.e. the function being called)
 */
BadReceiverError::BadReceiverError(std::string fun_name) { fn_name = fun_name; }

// "receiver of message 'foo' has to be an object"
void BadReceiverError::printTo(std::ostream &os) {
    os << "receiver of message '" << fn_name << "' has to be an object";
}

/* Bad Owner Error.
 *
 * CONDITION:
 *   when the owner of a member variable is not an object
 *   i.e.:  accessing XXX.vvv but XXX is not a class object
 * PARAMETERS:
 *   var_name - name of the member variable
 */
BadOwnerError::BadOwnerError(std::string var_name) { vr_name = var_name; }

// "owner of variable 'x' has to be an object"
void BadOwnerError::printTo(std::ostream &os) {
    os << "owner of variable '" << vr_name << "' has to be an object";
}

/* Not Method Error.
 *
 * CONDITION:
 *   calling a function but the symbol name does not correspond to a class
 * method NOTE: this error means the symbol is defined (although not function)
 * PARAMETERS:
 *   s     - the symbol with that name
 */
NotMethodError::NotMethodError(Symbol *s) {
    mind_assert(NULL != s && !s->isFunction());

    sm_name = s->getName();
    cs_name = std::string();
}

// "'harvest' is not a method in class 'Animal'"
void NotMethodError::printTo(std::ostream &os) {
    if (!cs_name.empty())
        os << "'" << sm_name << "' is not a method in class '" << cs_name
           << "'";
    else
        os << "'" << sm_name << "' is not a method";
}

/* Not Variable Error.
 *
 * CONDITION:
 *   accessing a variable but the symbol name does not correspond to a variable
 * NOTE:
 *   this error means the symbol is defined (although it is not variable)
 * PARAMETERS:
 *   s     - the symbol with that name
 */
NotVariableError::NotVariableError(Symbol *s) {
    mind_assert(NULL != s && !s->isVariable());

    sm_name = s->getName();
    cs_name = std::string();
}

// "'eat' is not a variable in class 'Animal'"
void NotVariableError::printTo(std::ostream &os) {
    if (!cs_name.empty())
        os << "'" << sm_name << "' is not a variable in class '" << (cs_name)
           << "'";
    else
        os << "'" << sm_name << "' is not a variable";
}

/* Symbol Not Found Error.
 *
 * CONDITION:
 *   accessing a variable or function, but the symbol is not defined
 * PARAMETERS:
 *   sym   - the problematic symbol name
 *   cs    - if given, it means the symbol is not member of the class
 */
SymbolNotFoundError::SymbolNotFoundError(std::string sym) {
    sm_name = sym;
    cs_name = std::string();
}

// "symbol 'WhoAmI' was not found in class 'AddressBook'"
void SymbolNotFoundError::printTo(std::ostream &os) {
    if (!cs_name.empty())
        os << "symbol '" << sm_name << "' was not found in class '" << (cs_name)
           << "'";
    else
        os << "symbol '" << sm_name << "' was not found";
}

/* Declaration Conflict Error.
 *
 * CONDITION:
 *   name confliction. possibilities are:
 *   1. two symbols with the same name in the SAME scope OR
 *   2. function in the base class and variable in the subclass owe the same
 * name OR
 *   3. variable in the base class and function in the subclass owe the same
 * name PARAMETERS: n     - the name s     - for case 1: the former declared one
 *           for case 2,3: the version the the base class
 */
DeclConflictError::DeclConflictError(std::string n, Symbol *s) {
    mind_assert(NULL != s);

    name = n;
    loc = s->getDefLocation();
}

// "declaration of 'x' conflicts with that at (4, 2)"
void DeclConflictError::printTo(std::ostream &os) {
    os << "declaration of '" << name << "' conflicts with that at " << loc;
}

/* Not Array Error.
 *
 * CONDITION:
 *   operator [] cannot be applied to anything other than arrays
 *   (i.e. you cannot access obj[4] where obj is a class object )
 */
NotArrayError::NotArrayError() {}

// "operator [] can only be applied to arrays"
void NotArrayError::printTo(std::ostream &os) {
    os << "operator [] can only be applied to arrays";
}

/* Unexpected Type Error.
 *
 * CONDITION:
 *   expecting some type, but a different one is given
 * NOTE:
 *   it is different from "Incompatible Error".
 * PARAMETERS:
 *   expected - the expected type
 *   given    - the type given (may be compatible with the expected type)
 */
UnexpectedTypeError::UnexpectedTypeError(Type *expected, Type *given) {
    bad = given;
    exp = expected;
}

// "'Int' is expected, but 'Bool' is given"
void UnexpectedTypeError::printTo(std::ostream &os) {
    os << "'" << exp << "' is expected, but '" << bad << "' is given";
}

/* Incompatible Type Error.
 *
 * CONDITION:
 *   expecting some type or one that is compatible of that type, but an
 *   incompatible type given (usually occurs in ASSIGN, RETURN, CALL)
 * PARAMETERS:
 *   expected - the expected type
 *   given    - the given one
 */
IncompatibleError::IncompatibleError(Type *expected, Type *given) {
    bad = given;
    exp = expected;
}

// "'Int' is incompatible with 'Bool'"
void IncompatibleError::printTo(std::ostream &os) {
    os << "'" << bad << "' is incompatible with '" << exp << "'";
}

/* Bad Equal Error.
 *
 * CONDITION:
 *   types of the operands on both sides of the == operator does not match
 * NOTE:
 *   it is different from Unexpected Type Error
 * PARAMETERS:
 *   left  - the type on the left hand side
 *   right - the type on the right hand side
 */
BadEqualError::BadEqualError(Type *left, Type *right) {
    t1 = left;
    t2 = right;
}

// "incompatible types for operator ==: 'Int' vs. 'Bool'"
void BadEqualError::printTo(std::ostream &os) {
    os << "incompatible types for operator ==: '" << t1 << "' vs. '" << t2
       << "'";
}

/* Bad Testing Expression Error.
 *
 * CONDITION:
 *   the testing expression of "if" or "while" statement is not of Bool
 */
BadTestExprError::BadTestExprError() {}

// "test expression must be bool"
void BadTestExprError::printTo(std::ostream &os) {
    os << "test expression must be bool";
}

ZeroLengthedArrayError::ZeroLengthedArrayError() {}

void ZeroLengthedArrayError::printTo(std::ostream &os) {
    os << "Zero-lengthed array is not allowed";
}
