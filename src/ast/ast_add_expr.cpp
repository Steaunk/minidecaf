/*****************************************************
 *  Implementation of "AddExpr".
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

/* Creates a new AddExpr node.
 *
 * PARAMETERS:
 *   op1     - left operand
 *   op2     - right operand
 *   l       - position in the source text
 */
AddExpr::AddExpr(Expr *op1, Expr *op2, Location *l) {

    setBasicInfo(ADD_EXPR, l);

    e1 = op1;
    e2 = op2;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void AddExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void AddExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << e1;
    newLine(os);
    os << e2 << ")";
    decIndent(os);
}
