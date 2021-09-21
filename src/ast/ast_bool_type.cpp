/*****************************************************
 *  Implementation of "BoolType".
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

/* Creates a new BoolType node.
 *
 * PARAMETERS:
 *   l       - position in the source text
 */
BoolType::BoolType(Location *l) { setBasicInfo(BOOL_TYPE, l); }

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void BoolType::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void BoolType::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    os << ")";
    decIndent(os);
}
