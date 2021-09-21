/*****************************************************
 *  Implementation of "IntConst".
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

/* Creates a new IntConst node.
 *
 * PARAMETERS:
 *   v       - the integer constant
 *   l       - position in the source text
 */
IntConst::IntConst(int v, Location *l) {

    setBasicInfo(INT_CONST, l);

    value = v;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void IntConst::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void IntConst::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    os << " " << value << ")";
    decIndent(os);
}
