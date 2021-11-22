/*****************************************************
 *  Definition of the AST visitor pattern.
 *
 *  It is only an interface (we provide an empty body
 *  just to make your life easier..).
 *
 *  Keltin Leung 
 */

#ifndef __MIND_VISITOR__
#define __MIND_VISITOR__

#include "ast/ast.hpp"

namespace mind {

namespace ast {

/*  AST Visitor Pattern.
 *
 *  HOW TO USE:
 *    1. Create a subclass of Visitor;
 *    2. Implement visit(ast::Program*)  (required);
 *    3. Implement the other visiting functions as you need;
 *    4. We provide an empty function body ({  }) instead of
 *       setting it pure (= 0) so that you do not need to
 *       implement all the functions as you do with a pure
 *       interface.
 */
class Visitor {
  public:
    // Expressions
    virtual void visit(CallExpr *) {}
    virtual void visit(AddExpr *) {}
    virtual void visit(AndExpr *) {}
    virtual void visit(AssignExpr *) {}
    virtual void visit(IfExpr *) {}
    virtual void visit(OrExpr *) {}
    virtual void visit(BoolConst *) {}
    virtual void visit(DivExpr *) {}
    virtual void visit(EquExpr *) {}
    virtual void visit(GeqExpr *) {}
    virtual void visit(GrtExpr *) {}
    virtual void visit(NeqExpr *) {}
    virtual void visit(IntConst *) {}
    virtual void visit(LeqExpr *) {}
    virtual void visit(LesExpr *) {}
    virtual void visit(LvalueExpr *) {}
    virtual void visit(ModExpr *) {}
    virtual void visit(MulExpr *) {}
    virtual void visit(NegExpr *) {}
    virtual void visit(NotExpr *) {}
    virtual void visit(BitNotExpr *) {}
    virtual void visit(SubExpr *) {}
    virtual void visit(IndexExpr *) {}

    // Lvalues
    virtual void visit(VarRef *) {}
    // Types
    virtual void visit(BoolType *) {}
    virtual void visit(IntType *) {}

    // Statements
    virtual void visit(ExprStmt *) {}
    virtual void visit(CompStmt *) {}
    virtual void visit(WhileStmt *) {}
    virtual void visit(ForStmt *) {}
    virtual void visit(EmptyStmt *) {}
    virtual void visit(BreakStmt *) {}
    virtual void visit(ContinueStmt *) {}
    virtual void visit(IfStmt *) {}
    virtual void visit(ReturnStmt *) {}
    // Declarations
    virtual void visit(Program *) = 0;
    virtual void visit(FuncDefn *) {}
    virtual void visit(VarDecl *) {}

    virtual ~Visitor() {}
};
} // namespace ast
} // namespace mind

#endif // __MIND_VISITOR__
