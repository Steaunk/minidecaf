/*****************************************************
 *  Implementation of "OrExpr".
 *
 *  Please refer to ast/ast.hpp for the definition.
 */

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;

/* Creates a new OrExpr node.
 *
 * PARAMETERS:
 *   op1     - left operand
 *   op2     - right operand
 *   l       - position in the source text
 */
OrExpr::OrExpr(Expr *op1, Expr *op2, Location *l) {

    setBasicInfo(OR_EXPR, l);

    e1 = op1;
    e2 = op2;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void OrExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void OrExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << e1;
    newLine(os);
    os << e2 << ")";
    decIndent(os);
}
