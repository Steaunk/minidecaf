
#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;

/* Creates a new CompStmt node.
 *
 * PARAMETERS:
 *   slist   - list of the statements
 */
CompStmt::CompStmt(StmtList *slist, Location *l) {
    setBasicInfo(COMP_STMT, l);
    stmts = slist;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void CompStmt::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void CompStmt::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << stmts << ")";
    decIndent(os);
}