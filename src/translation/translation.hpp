/*****************************************************
 *  Definition of the two AST translation passes.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_TRANSLATION__INTERNAL__
#define __MIND_TRANSLATION__INTERNAL__

#include "3rdparty/vector.hpp"
#include "ast/visitor.hpp"
#include "define.hpp"

// we assume the target machine is a 32-bit machine for simplicity.
#define POINTER_SIZE 4
#define WORD_SIZE 4

namespace mind {

class Translation : public ast::Visitor {
  public:
    Translation(tac::TransHelper *);

    virtual void visit(ast::Program *);
    virtual void visit(ast::FuncDefn *);
    virtual void visit(ast::AssignExpr *);
    virtual void visit(ast::CompStmt *);
    virtual void visit(ast::ExprStmt *);
    virtual void visit(ast::IfStmt *);
    virtual void visit(ast::ReturnStmt *);
    virtual void visit(ast::CallExpr *);
    virtual void visit(ast::EquExpr *);
    virtual void visit(ast::NeqExpr *);
    virtual void visit(ast::AndExpr *);
    virtual void visit(ast::OrExpr *);
    virtual void visit(ast::GrtExpr *);
    virtual void visit(ast::GeqExpr *);
    virtual void visit(ast::AddExpr *);
    virtual void visit(ast::SubExpr *);
    virtual void visit(ast::MulExpr *);
    virtual void visit(ast::DivExpr *);
    virtual void visit(ast::ModExpr *);
    virtual void visit(ast::IndexExpr *);
    virtual void visit(ast::IntConst *);
    virtual void visit(ast::NegExpr *);
    virtual void visit(ast::BitNotExpr *);
    virtual void visit(ast::NotExpr *);
    virtual void visit(ast::IfExpr *);
    virtual void visit(ast::LvalueExpr *);
    virtual void visit(ast::VarRef *);
    virtual void visit(ast::VarDecl *);
    virtual void visit(ast::WhileStmt *);
    virtual void visit(ast::ForStmt *);
    virtual void visit(ast::BreakStmt *);
    virtual void visit(ast::ContinueStmt *);

    virtual ~Translation() {}

  private:
    tac::TransHelper *tr;
    tac::Label current_break_label, current_continue_label;
};
} // namespace mind

#endif // __MIND_TRANSLATION__INTERNAL__
