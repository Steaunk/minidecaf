/*****************************************************
 *  Implementation of "LvalueExpr".
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

/* Creates a new LvalueExpr node.
 *
 * PARAMETERS:
 *   lv      - the left value
 *   l       - position in the source text
 */
LvalueExpr::LvalueExpr(Lvalue *lv, Location *l) {

    setBasicInfo(LVALUE_EXPR, l);

    lvalue = lv;
    // rvalue = NULL;
}
// LvalueExpr::LvalueExpr (Lvalue* lv, Expr* rv,
// 				  Location* l){
//   setBasicInfo(LVALUE_EXPR, l);

//   lvalue = lv;
//  // rvalue = rv;
// }
/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void LvalueExpr::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void LvalueExpr::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    // if(rvalue == NULL){
    os << lvalue << ")";
    //}
    // else{
    //   os << lvalue << "=";

    //   newLine(os);
    //   os << rvalue <<")";
    // }
    decIndent(os);
}
// void
// AssignStmt::dumpTo (std::ostream& os) {
//   ASTNode::dumpTo(os);
//   newLine(os);
//   os << left;

//   newLine(os);
//   os << e << ")";
//   decIndent(os);
// }