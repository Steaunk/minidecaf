/*****************************************************
 *  Global Declarations.
 *
 *  This file contains declaration of:
 *    1. the ID() and STR() function for manipulating
 *       strings;
 *    2. a set of overloaded output operators;
 *    3. indentation management (you won't use it);
 *    4. the identifier string of "main" - ID_MAIN_FUNC;
 *    5. the global symbol table stack;
 *    6. error management (including assertion);
 *    7. forward declaration of most classes;
 *    8. boehm garbage collector support.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_CONFIG__
#define __MIND_CONFIG__

#include "3rdparty/boehmgc.hpp"
#include "define.hpp"
#include "error.hpp"

#include <cstring>
#include <iostream>
#include <memory>

/* declaration of some global data & functions you will use */
namespace mind {

extern scope::ScopeStack *scopes;

/* Output Functions */
std::ostream &operator<<(std::ostream &, Location *);
std::ostream &operator<<(std::ostream &, ast::ASTNode *);
std::ostream &operator<<(std::ostream &, ast::FuncList *);
std::ostream &operator<<(std::ostream &, ast::VarList *);
std::ostream &operator<<(std::ostream &, ast::StmtList *);
std::ostream &operator<<(std::ostream &, ast::ExprList *);
std::ostream &operator<<(std::ostream &, ast::DimList *);
std::ostream &operator<<(std::ostream &, ast::FuncOrGlobalList *);
std::ostream &operator<<(std::ostream &, type::Type *);
std::ostream &operator<<(std::ostream &, scope::Scope *);
std::ostream &operator<<(std::ostream &, symb::Symbol *);
std::ostream &operator<<(std::ostream &, tac::Tac *);
std::ostream &operator<<(std::ostream &, tac::Temp);
std::ostream &operator<<(std::ostream &, tac::Label);
std::ostream &operator<<(std::ostream &, tac::VTable);
std::ostream &operator<<(std::ostream &, tac::Functy);

/* Output Indentation */
void newLine(std::ostream &);
void incIndent(std::ostream &);
void decIndent(std::ostream &);
} // namespace mind

#endif // __MIND_CONFIG__
