/*****************************************************
 *  Error Management of the Mind Compiler.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_ERROR__
#define __MIND_ERROR__

#include "define.hpp"

#include <iostream>

// Assertion Support
#define mind_assert(e)                                                         \
    (e ? (void)0 : mind::err::bad_assertion(__STRING(e), __FILE__, __LINE__))

namespace mind {

/* I suggest you refer to error.cpp for details.
 */
namespace err {
// compiler errors, all errors are subclass of it
class MindError {
  public:
    virtual void printTo(std::ostream &) = 0;
    virtual ~MindError() {}
};

// issues an error
void issue(Location *, MindError *);
// gets the number of errors having been issued so far
int numOfErrors(void);
// prints a debug message
void debug(const char *msg, ...);
// throws an assertion error (and exit)
void bad_assertion(const char *, const char *, int);
// confirms there has not been any error so far
void checkPoint(void);

// 0: Unrecognized Character Error
class UnrecogCharError : public MindError {
  public:
    UnrecogCharError(char);
    virtual void printTo(std::ostream &);

  private:
    char ch;
};

// 1: Integer Literal is Too Large Error
class IntTooLargeError : public MindError {
  public:
    IntTooLargeError();
    virtual void printTo(std::ostream &);
};
class CommentNotClosedError : public MindError {
  public:
    CommentNotClosedError();
    virtual void printTo(std::ostream &);
};

// 1: Syntax Error
class SyntaxError : public MindError {
  private:
    std::string msg;

  public:
    SyntaxError(std::string msg);
    virtual void printTo(std::ostream &);
};

// 2: Bad Argument Count Error
class BadArgCountError : public MindError {
  public:
    BadArgCountError(symb::Function *);
    virtual void printTo(std::ostream &);

  private:
    std::string fn_name;
    int arg_count;
};

// 3: Bad Print Statement Error
class BadPrintStmtError : public MindError {
  public:
    BadPrintStmtError();
    virtual void printTo(std::ostream &);
};

// 4: Bad Message Receiver Error
class BadReceiverError : public MindError {
  public:
    BadReceiverError(std::string);
    virtual void printTo(std::ostream &);

  private:
    std::string fn_name;
};

// 5: Bad Varible Owner Error
class BadOwnerError : public MindError {
  public:
    BadOwnerError(std::string);
    virtual void printTo(std::ostream &);

  private:
    std::string vr_name;
};

// 8: Symbol is Not Method Error
class NotMethodError : public MindError {
  public:
    NotMethodError(symb::Symbol *);
    virtual void printTo(std::ostream &);

  private:
    std::string sm_name;
    std::string cs_name;
};

// 9: Symbol is Not Variable Error
class NotVariableError : public MindError {
  public:
    NotVariableError(symb::Symbol *);
    virtual void printTo(std::ostream &);

  private:
    std::string sm_name;
    std::string cs_name;
};

// 10: Symbol Not Found Error
class SymbolNotFoundError : public MindError {
  public:
    SymbolNotFoundError(std::string);
    virtual void printTo(std::ostream &);

  private:
    std::string sm_name;
    std::string cs_name;
};

// 13: Declaration Conflict Error
class DeclConflictError : public MindError {
  public:
    DeclConflictError(std::string, symb::Symbol *);
    virtual void printTo(std::ostream &);

  private:
    std::string name;
    Location *loc;
};

// 17: Not Array Error
class NotArrayError : public MindError {
  public:
    NotArrayError();
    virtual void printTo(std::ostream &);
};

// 18: Unexpected Type Error
class UnexpectedTypeError : public MindError {
  public:
    UnexpectedTypeError(type::Type *, type::Type *);
    virtual void printTo(std::ostream &);

  private:
    type::Type *bad;
    type::Type *exp;
};

// 19: Incompatible Type Error
class IncompatibleError : public MindError {
  public:
    IncompatibleError(type::Type *, type::Type *);
    virtual void printTo(std::ostream &);

  private:
    type::Type *bad;
    type::Type *exp;
};

// 20: Bad == operator Error
class BadEqualError : public MindError {
  public:
    BadEqualError(type::Type *, type::Type *);
    virtual void printTo(std::ostream &);

  private:
    type::Type *t1;
    type::Type *t2;
};

// 21: Bad Testing Expression Error
class BadTestExprError : public MindError {
  public:
    BadTestExprError();
    virtual void printTo(std::ostream &);
};

class ZeroLengthedArrayError : public MindError {
  public:
    ZeroLengthedArrayError();
    virtual void printTo(std::ostream &);
};

} // namespace err
} // namespace mind

#endif // __MIND_ERROR__
