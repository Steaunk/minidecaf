/*****************************************************
 *  Implementation of "NegExpr".
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

/* Creates a new NegExpr node.
 *
 * PARAMETERS:
 *   e0      - the operand
 *   l       - position in the source text
 */
NegExpr::NegExpr(Expr *e0, Location *l) {

    setBasicInfo(NEG_EXPR, l);

    e = e0;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void NegExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void NegExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << e << ")";
    decIndent(os);
}
