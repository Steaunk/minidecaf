/*****************************************************
 *  Implementation of the "ASTNode" abstract class.
 *
 *  Keltin Leung 
 */

#include "ast/ast.hpp"
#include "config.hpp"
#include "location.hpp"
#include "options.hpp"

using namespace mind;
using namespace mind::ast;

/*  Names of the AST nodes.
 *
 *  NOTE: The order of node_name's must be the same with that of NodeType's.
 */
const char *ASTNode::node_name[] = {"add",
                                    "and",
                                    "assign",
                                    "bconst",
                                    "bitnot",

                                    "btype",
                                    "break",
                                    "call",
                                    "comp",
                                    "div",

                                    "equ",
                                    "empty",
                                    "expr",
                                    "func",
                                    "geq",

                                    "grt",
                                    "if",
                                    "if",
                                    "iconst",
                                    "itype",
                                    "leq",
                                    "les",
                                    "lvalue",
                                    "mod",
                                    "mul",
                                    "neg",
                                    "neq",
                                    "not",
                                    "or",
                                    "program",
                                    "return",
                                    "sub",
                                    "var",
                                    "varref",
                                    "while",
                                    "FuncOrDecl"};

/*  Whether to print the decorated abstract syntax tree.
 */
bool print_decorated_ast = false;

/*  Sets the basic information of a node.
 *
 *  PARAMETERS:
 *    k     - the node kind
 *    l     - position in the source text
 */
void ASTNode::setBasicInfo(NodeType k, Location *l) {
    kind = k;
    loc = l;
}

/*  Gets the node kind.
 *
 *  RETURNS:
 *    the node kind
 */
ASTNode::NodeType ASTNode::getKind(void) { return kind; }

/*  Gets the source text location of this node
 *
 *  RETURNS:
 *    the source text location
 */
Location *ASTNode::getLocation(void) { return loc; }

/*  Dumps this node to an output stream.
 *
 *  PARAMETERS:
 *    os    - the output stream
 */
void ASTNode::dumpTo(std::ostream &os) {
    os << "(" << node_name[kind];
    if (Option::getLevel() != Option::PARSER)
        os << " (" << loc->line << " " << loc->col << ")";
    incIndent(os);
}

/*  Outputs an ASTNode.
 *
 *  PARAMETERS:
 *    os    - the output stream
 *    p     - the ASTNode
 *  RETURNS:
 *    the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, ASTNode *p) {
    if (p == NULL)
        os << "!!NULL!!";
    else
        p->dumpTo(os);
    return os;
}
/*  Outputs a FuncList.
 *
 *  PARAMETERS:
 *    os    - the output stream
 *    l     - the Func list
 *  RETURNS:
 *    the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, FuncList *l) {
    os << "[";
    if (!l->empty()) {
        incIndent(os);
        os << " ";
        FuncList::iterator it = l->begin();
        os << *it;
        while (++it != l->end()) {
            newLine(os);
            os << *it;
        }
        decIndent(os);
        newLine(os);
    }
    os << "]";

    return os;
};

/*  Outputs a VarList.
 *
 *  PARAMETERS:
 *    os    - the output stream
 *    l     - the VarDecl list
 *  RETURNS:
 *    the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, VarList *l) {
    os << "[";
    if (!l->empty()) {
        incIndent(os);
        os << " ";
        VarList::iterator it = l->begin();
        os << *it;
        while (++it != l->end()) {
            newLine(os);
            os << *it;
        }
        decIndent(os);
        newLine(os);
    }
    os << "]";

    return os;
}

/*  Outputs a StmtList.
 *
 *  PARAMETERS:
 *    os    - the output stream
 *    l     - the Statement list
 *  RETURNS:
 *    the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, StmtList *l) {
    os << "[";
    if (!l->empty()) {
        incIndent(os);
        os << " ";
        StmtList::iterator it = l->begin();
        os << *it;
        while (++it != l->end()) {

            newLine(os);
            os << *it;
        }
        decIndent(os);
        newLine(os);
    }
    os << "]";

    return os;
}

/*  Outputs an ExprList .
 *
 *  PARAMETERS:
 *    os    - the output stream
 *    l     - the Expr list
 *  RETURNS:
 *    the output stream
 */
std::ostream &mind::operator<<(std::ostream &os, ExprList *l) {
    os << "[";
    if (!l->empty()) {
        incIndent(os);
        os << " ";
        ExprList::iterator it = l->begin();
        os << *it;
        while (++it != l->end()) {
            newLine(os);
            os << *it;
        }
        decIndent(os);
        newLine(os);
    }
    os << "]";

    return os;
}
std::ostream &mind::operator<<(std::ostream &os, DimList *l) {
    os << "[";
    if (!l->empty()) {
        incIndent(os);
        os << " ";
        DimList::iterator it = l->begin();
        os << *it;
        while (++it != l->end()) {
            newLine(os);
            os << *it;
        }
        decIndent(os);
        newLine(os);
    }
    os << "]";

    return os;
}
std::ostream &mind::operator<<(std::ostream &os, FuncOrGlobalList *l) {
    os << "[";
    if (!l->empty()) {
        incIndent(os);
        os << " ";
        FuncOrGlobalList::iterator it = l->begin();
        os << *it;
        while (++it != l->end()) {

            newLine(os);
            os << *it;
        }
        decIndent(os);
        newLine(os);
    }
    os << "]";

    return os;
}