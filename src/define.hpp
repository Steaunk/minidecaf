/*****************************************************
 *  Forward declarations of the classes.
 *
 *  This file is used to resolve the "cyclic reference"
 *  dilemma in C++.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_DEFINE__
#define __MIND_DEFINE__

#ifndef MIND_AST_DEFINED
#include "3rdparty/list.hpp"
#endif

namespace mind {
/* String ID */
typedef unsigned long SID;

#ifndef MIND_LOCATION_DEFINED
struct Location;
#endif

#ifndef MIND_TYPE_DEFINED
namespace type {
class Type;

class ArrayType;
class BaseType;
class FuncType;
} // namespace type
#endif

#ifndef MIND_SCOPE_DEFINED
namespace scope {
class Scope;

class FuncScope;
class GlobalScope;
} // namespace scope
#endif

#ifndef MIND_SCOPESTACK_DEFINED
namespace scope {
class ScopeStack;
}
#endif

#ifndef MIND_SYMB_DEFINED
namespace symb {
class Symbol;

class Function;
class Variable;
} // namespace symb
#endif

#ifndef MIND_AST_DEFINED
namespace ast {
class Visitor;
class ASTNode;

class AddExpr;
class AndExpr;
class AssignExpr;
class BoolConst;
class BoolType;
class DivExpr;
class EquExpr;
class Expr;
class ExprStmt;
class FuncDefn;
class IfStmt;
class IntConst;
class IntType;
class LeqExpr;
class LesExpr;
class Lvalue;
class LvalueExpr;
class ModExpr;
class MulExpr;
class NegExpr;
class NotExpr;
class Program;
class ReadExpr;
class ReturnStmt;
class Statement;
class SubExpr;
class Type;
class VarDecl;
class VarRef;
class WhileStmt;
class CompStmt;
class FuncOrGlobal;

// the lists are not ASTNode
typedef util::List<FuncDefn *> FuncList;        // list of Function
typedef util::List<VarDecl *> VarList;          // list of VarDecl
typedef util::List<int> DimList;                // list of VarDecl
typedef util::List<Statement *> StmtList;       // list of Statement
typedef util::List<Expr *> ExprList;            // list of Expr
typedef util::List<ASTNode *> FuncOrGlobalList; // list of Expr

} // namespace ast
#endif

#ifndef MIND_TAC_DEFINED
namespace tac {
struct LibFunc;
struct TempObject;
struct LabelObject;
struct VTableObject;
struct FunctyObject;
typedef struct TempObject *Temp;
typedef struct LabelObject *Label;
typedef struct VTableObject *VTable;
typedef struct FunctyObject *Functy;

struct Tac;
struct Piece;
} // namespace tac
#endif

#ifndef MIND_TRANSHELPER_DEFINED
namespace tac {
class TransHelper;
}
#endif

#ifndef MIND_FLOWGRAPH_DEFINED
namespace tac {
struct BasicBlock;
class FlowGraph;
} // namespace tac
#endif

#ifndef MIND_MACHDESC_DEFINED
namespace assembly {
struct Instr;
class MachineDesc;
} // namespace assembly
#endif

#ifndef MIND_OFFSETCOUNTER_DEFINED
namespace assembly {
class OffsetCounter;
}
#endif

#ifndef RISCV_COMPONENTS_DEFINED
namespace assembly {
struct RiscvReg;
struct RiscvInstr;
class RiscvDesc;
} // namespace assembly
#endif

} // namespace mind

#endif // __MIND_DEFINE__
