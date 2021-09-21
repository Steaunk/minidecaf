/*****************************************************
 *  Implementation of "ReturnStmt".
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

/* Creates a new ReturnStmt node.
 *
 * PARAMETERS:
 *   e0      - the return value expression
 *   l       - position in the source text
 */
ReturnStmt::ReturnStmt(Expr *e0, Location *l) {

    setBasicInfo(RETURN_STMT, l);

    e = e0;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void ReturnStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void ReturnStmt::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << e << ")";
    decIndent(os);
}
