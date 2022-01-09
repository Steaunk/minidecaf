/*****************************************************
 *  Implementation of "VarRef".
 *
 *  Please refer to ast/ast.hpp for the definition.
 *
 *  Keltin Leung 
 *
 *  removed owner
 */

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;

/* Creates a new VarRef node.
 *
 * PARAMETERS:
 *   n       - name of the referenced variable
 *   l       - position in the source text
 */
VarRef::VarRef(std::string n, Location *l) {

    setBasicInfo(VAR_REF, l);

    var = n;
    expr = NULL;

    ATTR(sym) = NULL;
}

VarRef::VarRef(std::string n, IndexExpr *expr, Location *l){

    setBasicInfo(VAR_REF, l);
    
    var = n;
    this->expr = expr;

    ATTR(sym) = NULL;
}

/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void VarRef::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void VarRef::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    os << " " << '"' << var << '"';
    newLine(os);
    if(NULL != expr){
        os << expr;
    }
    //if (NULL != owner)
    //    os << owner << ")";
    //else
    //    os << "())";
    os << ")";
    decIndent(os);
}
