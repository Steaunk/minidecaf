/*****************************************************
 *  Implementation of "IfStmt".
 *
 *  Please refer to ast/ast.hpp for the definition.
 *
 *  Keltin Leung 
 */

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;

/* Creates a new IfStmt node.
 *
 * PARAMETERS:
 *   cond    - the test expression
 *   true_branch
 *           - the true branch
 *   false_branch
 *           - the false branch
 *   l       - position in the source text
 */
IfStmt::IfStmt(Expr *cond, Statement *true_branch, Statement *false_branch,
               Location *l) {

    setBasicInfo(IF_STMT, l);

    condition = cond;
    true_brch = true_branch;
    false_brch = false_branch;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void IfStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void IfStmt::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << condition;

    newLine(os);
    os << true_brch;
    newLine(os);
    os << "else";
    newLine(os);
    os << false_brch << ")";
    decIndent(os);
}

/* Creates a new IfExpr node.
 *
 * PARAMETERS:
 *   cond    - the test expression
 *   true_branch
 *           - the true branch
 *   false_branch
 *           - the false branch
 *   l       - position in the source text
 */
IfExpr::IfExpr(Expr *cond, Expr *true_branch, Expr *false_branch, Location *l) {

    setBasicInfo(IF_EXPR, l);

    condition = cond;
    true_brch = true_branch;
    false_brch = false_branch;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void IfExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void IfExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << condition;

    newLine(os);
    os << "then" << true_brch;

    newLine(os);
    os << "else" << false_brch << ")";
    decIndent(os);
}
