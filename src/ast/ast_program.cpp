/*****************************************************
 *  Implementation of "Program".
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

/* Creates a new Program node.
 *
 * PARAMETERS:
 *   flist   - list of the contained function definitions
 *   l       - position in the source text
 */
Program::Program(ASTNode *first, Location *l) {

    setBasicInfo(PROGRAM, l);
    func_and_globals = new FuncOrGlobalList(first);
    ATTR(main) = NULL;
    ATTR(gscope) = NULL;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void Program::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void Program::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << func_and_globals << ")";

    decIndent(os);
}
