/*****************************************************
 *  Semantic Types of Mind.
 *
 *  This file defines four semantic types for you:
 *    1. BaseType:  Int, Error
 *    2. ArrayType: representing the types of arrays
 *    3. FuncType:  representing the types of functions
 *
 *  Keltin Leung 
 */

#ifndef __MIND_TYPE__
#define __MIND_TYPE__

#include "3rdparty/list.hpp"
#include "define.hpp"

#include <iostream>

namespace mind {

#define MIND_TYPE_DEFINED
namespace type {

/* Semantic Types.
 *
 * These classes represent the semantic types of the symbols.
 * NOTE:
 *   don't get confused with the XXXType AST nodes
 */
class Type {
  public:
    // Tests whether this type is BaseType
    virtual bool isBaseType(void);
    // Tests whether this type is ArrayType
    virtual bool isArrayType(void);
    // Tests whether this type is FuncType
    virtual bool isFuncType(void);
    // Get the size of this type
    virtual int getSize() { return 0; }
    // Tests whether this type is compatible with the given type
    // NOTE:
    //   if "a <- b" is legal, then type(b) is compatible with type(a)
    virtual bool compatible(Type *) = 0;
    // Tests whether this type is equal to the given type
    virtual bool equal(Type *) = 0;
    // Prints this type
    virtual void dump(std::ostream &) = 0;

    virtual ~Type() {}
};

/* The base types.
 *
 * Base types include: Int, Error.
 * NullType is the type of the Null expression;
 * ErrorType means "due to some previous error, the exact type is undecidable"
 */
class BaseType : public Type {
  private:
    // name of the type
    const char *type_name;
    // private instances of the base types
    static BaseType _int, _error;
    // don't call the constructor explictly
    BaseType(const char *);

  public:
    // Tests whether it is a BaseType
    virtual bool isBaseType(void);
    // Get the size of this type
    virtual int getSize();
    // Tests whether this type is compatible with the given type
    virtual bool compatible(Type *);
    // Tests whether this type is equal to the given type
    virtual bool equal(Type *);
    // Prints this type object
    virtual void dump(std::ostream &);

    // the Int type instance (use BaseType::Int to refer)
    static BaseType *const Int;
    // the Error type instance (use BaseType::Error to refer)
    static BaseType *const Error;
};

/* The array type.
 *
 * ArrayType is used as the type of an array.
 * NOTE:
 *   using BaseType::Error as the element type is illegal
 */
class ArrayType : public Type {
  private:
    // the element type
    Type *element_type;
    int length;

  public:
    // Constructor
    ArrayType(Type *, int length);
    // Gets the element type (a.k.a. "the base type of an array")
    Type *getElementType(void);
    // Gets the array length
    int getLength(void);
    // Tests whether it is ArrayType
    virtual bool isArrayType(void);
    // Get the size of this type
    virtual int getSize();
    // Tests whether it is compatible with the given type
    virtual bool compatible(Type *);
    // Tests whether it is equal to the given type
    virtual bool equal(Type *);
    // Prints this type object
    virtual void dump(std::ostream &);
};

/* The function type signature.
 *
 * FuncType is used to denote the type of a function.
 * NOTE:
 *   "result type" of a function is different from the funtion type
 */
class FuncType : public Type {
  private:
    // the result type
    Type *result_type;
    // the type list of the arguments (order preserved)
    util::List<Type *> *arglist;

  public:
    // Constructor
    FuncType(Type *result);
    // Gets the result type
    Type *getResultType(void);
    // Gets the argument type list
    util::List<Type *> *getArgList(void);
    // Gets the number of the arguments
    size_t numOfParameters(void);
    // Appends an argument type to the argument type list
    void appendParameter(Type *);
    // Tests whether this type is a FuncType
    virtual bool isFuncType(void);
    // Tests whether this type is compatible with the given type
    virtual bool compatible(Type *);
    // Tests whether this type is equal to the given type
    virtual bool equal(Type *);
    // Prints this type object
    virtual void dump(std::ostream &);
};
} // namespace type
} // namespace mind

#endif // __MIND_TYPE__
