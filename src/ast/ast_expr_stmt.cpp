/*****************************************************
 *  Implementation of "ExprStmt".
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

/* Creates a new ExprStmt node.
 *
 * PARAMETERS:
 *   e0      - the expression
 *   l       - position in the source text
 */
ExprStmt::ExprStmt(Expr *e0, Location *l) {

    setBasicInfo(EXPR_STMT, l);

    e = e0;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void ExprStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void ExprStmt::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << e << ")";
    decIndent(os);
}

/* Creates a new EmptyStmt node.
 *
 * PARAMETERS:
 *   l       - position in the source text
 */
EmptyStmt::EmptyStmt(Location *l) { setBasicInfo(EMPTY_STMT, l); }

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void EmptyStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void EmptyStmt::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    decIndent(os);
}
