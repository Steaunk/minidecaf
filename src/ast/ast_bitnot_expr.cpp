/*****************************************************
 *  Implementation of "BitNotExpr".
 *
 *  Please refer to ast/ast.hpp for the definition.
 *
 */

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;

/* Creates a new BitNotExpr node.
 *
 * PARAMETERS:
 *   e0      - the operand
 *   l       - position in the source text
 */
BitNotExpr::BitNotExpr(Expr *e0, Location *l) {

    setBasicInfo(BIT_NOT_EXPR, l);

    e = e0;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void BitNotExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void BitNotExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << e << ")";
    decIndent(os);
}
