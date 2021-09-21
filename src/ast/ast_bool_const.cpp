/*****************************************************
 *  Implementation of "BoolConst".
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

/* Creates a new BoolConst node.
 *
 * PARAMETERS:
 *   v       - the constant value
 *   l       - position in the source text
 */
BoolConst::BoolConst(bool v, Location *l) {

    setBasicInfo(BOOL_CONST, l);

    value = v;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void BoolConst::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void BoolConst::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    os << " " << value << ")";
    decIndent(os);
}
