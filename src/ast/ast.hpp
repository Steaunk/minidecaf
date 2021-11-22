/*****************************************************
 *  Definition of the AST nodes.
 *
 *  Please read the description of each class for details.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_AST__
#define __MIND_AST__

#include "define.hpp"
#include <iostream>
#include <string>

// the prefix of an ast attribution
#define ATTR(x) ast_attr_##x##_

namespace mind {

#define MIND_AST_DEFINED
namespace ast {

/* The base class of all AST nodes.
 *
 * NOTE: Don't instantiate this class.
 *       Please read the description of the subclasses.
 */
class ASTNode {
  public:
    // types of the AST nodes
    typedef enum {
        ADD_EXPR,
        AND_EXPR,
        ASSIGN_EXPR,
        BOOL_CONST,
        BIT_NOT_EXPR,
        BOOL_TYPE,
        BREAK_STMT,
        CONTINUE_STMT,
        CALL_EXPR,
        COMP_STMT,
        DIV_EXPR,
        EQU_EXPR,
        EMPTY_STMT,
        EXPR_STMT,
        FUNC_DEFN,
        GEQ_EXPR,
        GRT_EXPR,
        IF_STMT,
        IF_EXPR,
        INT_CONST,
        INT_TYPE,
        LEQ_EXPR,
        LES_EXPR,
        LVALUE_EXPR,
        MOD_EXPR,
        MUL_EXPR,
        NEG_EXPR,
        NEQ_EXPR,
        NOT_EXPR,
        OR_EXPR,
        PROGRAM,
        RETURN_STMT,
        SUB_EXPR,
        VAR_DECL,
        VAR_REF,
        WHILE_STMT,
        FOR_STMT,
        FOD,
        INDEX_EXPR
    } NodeType;

  protected:
    // names of each kind of the AST nodes
    static const char *node_name[];
    // kind of this node
    NodeType kind;
    // position in the source text
    Location *loc;
    // for subclass constructors only
    virtual void setBasicInfo(NodeType, Location *);

  public:
    // gets the node kind
    virtual NodeType getKind(void);
    // gets the location
    virtual Location *getLocation(void);
    // prints to the specified output stream
    virtual void dumpTo(std::ostream &);
    // for Visitor
    virtual void accept(Visitor *) = 0;
    // remember: let alone the memory deallocation stuff
    virtual ~ASTNode(void) {}
};

/* Node representing a type.
 *
 * NOTE:
 *   it is purely an interface.
 * IMPLEMENTATION:
 *   IntType, BoolType, ArrayType
 */
class Type : public ASTNode {
  public:
    type::Type *ATTR(type); // for semantic analysis
};

/* Node representing a statement.
 *
 * NOTE:
 *   it is purely an interface.
 * IMPLEMENTATION:
 *   AssignStmt, ReturnStmt,
 *   IfStmt, WhileStmt,
 */
class Statement : public ASTNode { /* pure interface */
};

/* Node representing an expression.
 *
 * NOTE:
 *   it is purely an interface.
 * IMPLEMENTATION:
 *   ...
 */
class EmptyStmt : public Statement {
  public:
    EmptyStmt(Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);
};
class Expr : public ASTNode {
  public:
    type::Type *ATTR(type); // for semantic analysis
    tac::Temp ATTR(val);    // for tac generation
};

/* Node representing a left-value expression (lvalue).
 *
 * NOTE:
 *   it is purely an interface.
 * IMPLEMENTATION:
 *   VarRef, ArrayRef
 */
class Lvalue : public ASTNode {
  public:
    enum {
        SIMPLE_VAR, // referencing simple variable
        ARRAY_ELE   // referencing array element
    } ATTR(lv_kind);

    type::Type *ATTR(type); // for semantic analysis
};

/* Node representing a program.
 *
 * SERIALIZED FORM:
 *   (program [ FUNC_1 FUNC_2 ... FUNC_N ])
 */
class Program : public ASTNode {
  public:
    Program(ASTNode *first, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    //   FuncList* functions;
    //   VarList* globals;
    FuncOrGlobalList *func_and_globals;
    symb::Function *ATTR(main);       // for semantic analysis
    scope::GlobalScope *ATTR(gscope); // for semantic analysis
};

/* Node representing a variable declaration.
 *
 * SERIALIZED FORM:
 *   (var NAME TYPE)
 */
class VarDecl : public Statement {
  public:
    VarDecl(std::string name, Type *type, Location *l);
    VarDecl(std::string name, Type *type, DimList *dim, Location *l);

    VarDecl(std::string name, Type *type, Expr *init, Location *l);
    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    std::string name;
    Type *type;
    Expr *init;
    DimList *dim;

    symb::Variable *ATTR(sym); // for semantic analysis
};

class FuncDefn : public ASTNode {
  public:
    FuncDefn(std::string name, Type *type, VarList *formals, StmtList *stmts,
             Location *l);
    FuncDefn(std::string name, Type *type, VarList *formals, EmptyStmt *empty,
             Location *l);
    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    std::string name;
    Type *ret_type;
    VarList *formals;
    StmtList *stmts;
    bool forward_decl; // is this FuncDefn a forward declaration or full
                       // definition?
    symb::Function *ATTR(sym); // for semantic analysis
};

class CallExpr : public Expr {
  public:
    CallExpr(ExprList *expr_list, std::string func, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

    std::string func;
    ExprList *expr_list;
    symb::Function *ATTR(sym); // for tac generation

};

class IndexExpr : public Expr {
  public:
    IndexExpr(Expr *, ExprList *, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

    ExprList *expr_list;
    DimList *ATTR(dim);
};

class FuncOrGlobal {
  public:
    FuncOrGlobal(FuncDefn *f, VarDecl *d) {
        func = f;
        decl = d;
    }
    FuncDefn *func; // assert ( func == NULL xor decl == NULL)
    VarDecl *decl;
};
class FOD {
  public:
    FOD(FuncList *f, VarList *d) {
        func = f;
        decl = d;
    }
    FuncList *func;
    VarList *decl;
};
/* Node representing the integer type.
 *
 * SERIALIZED FORM:
 *   (itype)
 */
class IntType : public Type {
  public:
    IntType(Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);
};

/* Node representing the Boolean type.
 *
 * SERIALIZED FORM:
 *   (btype)
 */
class BoolType : public Type {
  public:
    BoolType(Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);
};

/* Node representing an assignment statement.
 *
 * SERIALIZED FORM:
 *   (assign LEFT RIGHT)
 */
class AssignExpr : public Expr {
  public:
    AssignExpr(Lvalue *left, Expr *e, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Lvalue *left;
    Expr *e;
};

/* Node representing a return statement.
 *
 * SERIALIZED FORM:
 *   (return EXPR)
 */
class ReturnStmt : public Statement {
  public:
    ReturnStmt(Expr *e, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e;
};

/* Node representing a while statement.
 *
 * SERIALIZED FORM:
 *   (while CONDITION LOOP_BODY) 
 *   //(for INIT COND UPDATE LOOP_BODY)
 */
class WhileStmt : public Statement {
  public:
    WhileStmt(Expr *cond, Statement *loop_body, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *condition;
    Statement *loop_body;
};

/* Node representing a for statement.
 *
 * SERIALIZED FORM:
 *   (while CONDITION LOOP_BODY) 
 *   //(for INIT COND UPDATE LOOP_BODY)
 */
class ForStmt : public Statement {
  public:
    ForStmt(Statement *init, Expr *cond, Expr *update, Statement *loop_body, Location *l);
    ForStmt(Expr *init, Expr *cond, Expr *update, Statement *loop_body, Location *l);
    ForStmt(Expr *first_condition, Statement *loop_body, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *condition, *update, *first_condition;
    Statement *loop_body;
    ASTNode *init;
    scope::Scope *ATTR(scope);
};



/* Node representing an comp statement.
 *
 * SERIALIZED FORM:
 *   ( { StmtList } )
 */
class CompStmt : public Statement {
  public:
    CompStmt(StmtList *stmts, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    StmtList *stmts;

    scope::Scope *ATTR(scope);
};
/* Node representing an if statement.
 *
 * SERIALIZED FORM:
 *   (if CONDITION TRUE_BRANCH FALSE_BRANCH)
 */
class IfStmt : public Statement {
  public:
    IfStmt(Expr *cond, Statement *true_branch, Statement *false_branch,
           Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *condition;
    Statement *true_brch;
    Statement *false_brch;
};

/* Node representing an expression statement.
 *
 * SERIALIZED FORM:
 *   (expr EXPR)
 */
class ExprStmt : public Statement {
  public:
    ExprStmt(Expr *e, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e;
};

class BreakStmt : public Statement {
  public:
    BreakStmt(Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);
};

class ContinueStmt : public Statement {
  public:
    ContinueStmt(Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);
};
class ContStmt : public Statement {
  public:
    ContStmt(Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);
};

/* Node representing the reference to a variable.
 *
 * NOTE:
 *   (object == NULL) means there is no receiver.
 * SERIALIZED FORM:
 *   (varref VAR_NAME OBJECT_EXPR)
 *   (varref VAR_NAME ())
 */
class VarRef : public Lvalue {
  public:
    //	  VarRef (Expr* object, SID var_name,Location* l);
    VarRef(std::string var_name, Location *l);
    VarRef(std::string var_name, IndexExpr *expr, Location* l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *owner; // only to pass compilation, not used
    std::string var;
    IndexExpr *expr;

    symb::Variable *ATTR(sym); // for tac generation
};

class PointerRef : public Lvalue {
  public:
    PointerRef(Expr *pointer, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *pointer;

    symb::Variable *ATTR(sym); // for tac generation
};

/* Node representing an expression of lvalue.
 *
 * SERIALIZED FORM:
 *   (lvalue REFERENCE)
 */
class LvalueExpr : public Expr {
  public:
    LvalueExpr(Lvalue *lv, Location *l);
    //   LvalueExpr (Lvalue* lv, Expr* rv,
    // 			  Location* l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Lvalue *lvalue;
    // Expr* 	 rvalue;
};
/* Node representing an integer constant .
 *
 * SERIALIZED FORM:
 *   (iconst VALUE)
 */
class IntConst : public Expr {
  public:
    IntConst(int value, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    int value;
};

/* Node representing a Boolean constant .
 *
 * SERIALIZED FORM:
 *   (bconst VALUE)
 */
class BoolConst : public Expr {
  public:
    BoolConst(bool value, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    bool value;
};

/* Node representing the null expression .
 *
 * SERIALIZED FORM:
 *   (null)
 */
class NullExpr : public Expr {
  public:
    NullExpr(Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);
};

/* Node representing an addition.
 *
 * SERIALIZED FORM:
 *   (add EXPR1 EXPR2)
 */
class AddExpr : public Expr {
  public:
    AddExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a substraction.
 *
 * SERIALIZED FORM:
 *   (sub EXPR1 EXPR2)
 */
class SubExpr : public Expr {
  public:
    SubExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a multiplcation.
 *
 * SERIALIZED FORM:
 *   (mul EXPR1 EXPR2)
 */
class MulExpr : public Expr {
  public:
    MulExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a division.
 *
 * SERIALIZED FORM:
 *   (div EXPR1 EXPR2)
 */
class DivExpr : public Expr {
  public:
    DivExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a modular operation.
 *
 * SERIALIZED FORM:
 *   (mod EXPR1 EXPR2)
 */
class ModExpr : public Expr {
  public:
    ModExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};
/* Node representing a greater-than comparision.
 *
 * SERIALIZED FORM:
 *   (grt EXPR1 EXPR2)
 */
class GrtExpr : public Expr {
  public:
    GrtExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a greater-than-or-equal-to comparision.
 *
 * SERIALIZED FORM:
 *   (geq EXPR1 EXPR2)
 */
class GeqExpr : public Expr {
  public:
    GeqExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a less-than comparision.
 *
 * SERIALIZED FORM:
 *   (les EXPR1 EXPR2)
 */
class LesExpr : public Expr {
  public:
    LesExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a less-than-or-equal-to comparision.
 *
 * SERIALIZED FORM:
 *   (leq EXPR1 EXPR2)
 */
class LeqExpr : public Expr {
  public:
    LeqExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing an equivalence comparision.
 *
 * SERIALIZED FORM:
 *   (equ EXPR1 EXPR2)
 */
class EquExpr : public Expr {
  public:
    EquExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};
/* Node representing an non-equivalence comparision.
 *
 * SERIALIZED FORM:
 *   (neq EXPR1 EXPR2)
 */
class NeqExpr : public Expr {
  public:
    NeqExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a logical-and operation .
 *
 * SERIALIZED FORM:
 *   (and EXPR1 EXPR2)
 */
class AndExpr : public Expr {
  public:
    AndExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};

/* Node representing a logical-or operation .
 *
 * SERIALIZED FORM:
 *   (and EXPR1 EXPR2)
 */
class OrExpr : public Expr {
  public:
    OrExpr(Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e1;
    Expr *e2;
};
class IfExpr : public Expr {
  public:
    IfExpr(Expr *cond, Expr *e1, Expr *e2, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *condition;
    Expr *true_brch;
    Expr *false_brch;
};

/* Node representing a negating operation.
 *
 * SERIALIZED FORM:
 *   (neg EXPR)
 */
class NegExpr : public Expr {
  public:
    NegExpr(Expr *e, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e;
};
/* Node representing a logical-not operation.
 *
 * SERIALIZED FORM:
 *   (not EXPR)
 */
class NotExpr : public Expr {
  public:
    NotExpr(Expr *e, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e;
};
/* Node representing a bitwise-not operation.
 *
 * SERIALIZED FORM:
 *   (bitnot EXPR)
 */
class BitNotExpr : public Expr {
  public:
    BitNotExpr(Expr *e, Location *l);

    virtual void accept(Visitor *);
    virtual void dumpTo(std::ostream &);

  public:
    Expr *e;
};
extern bool print_decorated_ast;
} // namespace ast
} // namespace mind

#endif // __MIND_AST__
