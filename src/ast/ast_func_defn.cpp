/*****************************************************
 *  Implementation of "FuncDefn".
 *
 *  Please refer to ast/ast.hpp for the definition.
 *
 */

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "config.hpp"

using namespace mind;
using namespace mind::ast;

/* Creates a new FuncDefn node.
 *
 * PARAMETERS:
 *   n       - name of the function
 *   t       - result type of the function
 *   flist   - list of the formal parameters
 *   slist   - list of the statements in the function body
 *   l       - position in the source text
 */
FuncDefn::FuncDefn(std::string n, Type *t, VarList *flist, StmtList *slist,
                   Location *l) {

    setBasicInfo(FUNC_DEFN, l);

    name = n;
    ret_type = t;
    formals = flist;
    stmts = slist;
    forward_decl = false;
}
FuncDefn::FuncDefn(std::string n, Type *t, VarList *flist, EmptyStmt *empty,
                   Location *l) {
    setBasicInfo(FUNC_DEFN, l);

    name = n;
    ret_type = t;
    formals = flist;
    stmts = new ast::StmtList();
    forward_decl = true;
}
/* Visits the current node.
 *
 * PARAMETERS:
 *   v       - the visitor
 */
void FuncDefn::accept(Visitor *v) { v->visit(this); }

/* Prints the current AST node.
 *
 * PARAMETERS:
 *   os      - the output stream
 */
void FuncDefn::dumpTo(std::ostream &os) {
    ASTNode::dumpTo(os);
    newLine(os);
    os << '"' << name << '"' << " " << ret_type;

    newLine(os);
    os << formals;
    newLine(os);
    os << stmts << ")";
    decIndent(os);
}
