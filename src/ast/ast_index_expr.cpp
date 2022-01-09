/*****************************************************
 *  Implementation of "AssignStmt".
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

/* Creates a new AssignExpr node.
 *
 * PARAMETERS:
 *   lv      - the left value
 *   e0      - the expresion on the right side (i.e. right value)
 *   l       - position in the source text
 */
IndexExpr::IndexExpr(Expr *expr, ExprList *expr_list, Location *l) {

    setBasicInfo(INDEX_EXPR, l);
    this->expr_list = expr_list;
    this->expr_list->addAtHead(expr);
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void IndexExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void IndexExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    for(auto expr : *expr_list){
        os << "[" << expr << "]";
        newLine(os);
    }
    decIndent(os);
}
