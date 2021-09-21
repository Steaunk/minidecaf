/*****************************************************
 *  Implementation of "WhileStmt".
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

/* Creates a new WhileStmt node.
 *
 * PARAMETERS:
 *   cond    - the test expression
 *   body    - the loop body
 *   l       - position in the source text
 */
WhileStmt::WhileStmt(Expr *cond, Statement *body, Location *l) {

    setBasicInfo(WHILE_STMT, l);

    condition = cond;
    loop_body = body;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void WhileStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void WhileStmt::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << condition;

    newLine(os);
    os << loop_body << ")";
    decIndent(os);
}

/* Creates a new BreakStmt node.
 *
 * PARAMETERS:
 *   l       - position in the source text
 */
BreakStmt::BreakStmt(Location *l) { setBasicInfo(BREAK_STMT, l); }

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void BreakStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void BreakStmt::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    decIndent(os);
}
