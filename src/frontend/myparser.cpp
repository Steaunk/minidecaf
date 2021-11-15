
#include "myparser.hpp"
extern Token yylex();//get Next Token from flex-generated scanner
extern void set_scanner_input  (const char *filename); //tell the scanner to scan characters from "filename"
extern void close_scanner_input();
std::string TokenName[] = {  //string name for enumeration TokenType
    "identifier",
    "int_type","int_const",
    "return",
    "(", ")",
    "{", "}",
    ";", "=",
    "?",":",
    "and","or",
    "<",">","<=",">=","==","!=",
    "+","-","*","/","%",
    "~","!",
    "if", "else",
    "end"
};


const int MAX_Symbol = 36;
const int MAX_Token = 32;
// !NOTE: isFirst and isFollow is not complete, please modify the arrays as needed
bool isFirst[MAX_Symbol][MAX_Token];
bool isFollow[MAX_Symbol][MAX_Token];
static void init_first_set(){
  /*
 isFirst[SvmbolType::Primary][next.token.type]用来判断下一个token 是否在 FIRST(primary)中,以选择产生式unary:primary。
在同学们需要填空的代码中，待选择的多个产生式右端通常都以终结符开始，因此只需通过判断 next token 的终结符类型选择产生式即可，
无需用到isFirst集合。同时请注意【目前的isFirst集合并不完整】，如果想使用，请按照自己需求【初始化对应项的值】。
*/
  memset(isFirst,0,sizeof(isFirst));
  //incrementally build first set for Primary/Unary/Multiplicative/Additive/Relational
  isFirst[SymbolType::Primary][TokenType::ICONST] = true;
  isFirst[SymbolType::Primary][TokenType::IDENTIFIER] = true;
  isFirst[SymbolType::Primary][TokenType::LPAREN] = true;

  memcpy(isFirst[SymbolType::Unary],isFirst[SymbolType::Primary],sizeof(isFirst[0]));

  isFirst[SymbolType::Unary][TokenType::MINUS] = true;
  isFirst[SymbolType::Unary][TokenType::BNOT] = true;
  isFirst[SymbolType::Unary][TokenType::LNOT] = true;

  memcpy(isFirst[SymbolType::Multiplicative],isFirst[SymbolType::Unary],sizeof(isFirst[0]));

  memcpy(isFirst[SymbolType::Additive],isFirst[SymbolType::Multiplicative],sizeof(isFirst[0]));

  memcpy(isFirst[SymbolType::Relational],isFirst[SymbolType::Additive],sizeof(isFirst[0]));
  memcpy(isFirst[SymbolType::Equality],isFirst[SymbolType::Relational],sizeof(isFirst[0]));
  memcpy(isFirst[SymbolType::LogicalAnd],isFirst[SymbolType::Equality],sizeof(isFirst[0]));
  memcpy(isFirst[SymbolType::LogicalOr],isFirst[SymbolType::LogicalAnd],sizeof(isFirst[0]));
  memcpy(isFirst[SymbolType::Conditional],isFirst[SymbolType::LogicalOr],sizeof(isFirst[0]));
  memcpy(isFirst[SymbolType::Assignment],isFirst[SymbolType::Conditional],sizeof(isFirst[0]));
  memcpy(isFirst[SymbolType::Expression],isFirst[SymbolType::Assignment],sizeof(isFirst[0])); 


  memcpy(isFirst[SymbolType::Statement],isFirst[SymbolType::Expression],sizeof(isFirst[0])); 

  isFirst[SymbolType::Statement][TokenType::IF]=true;
  isFirst[SymbolType::Statement][TokenType::LBRACE]=true;
  isFirst[SymbolType::Statement][TokenType::SEMICOLON]=true;
  isFirst[SymbolType::Statement][TokenType::RETURN]=true;
  
  isFirst[SymbolType::VarDecl][TokenType::INT]=true;

}

static void init_follow_set(){
  //we only init what we use, isFollow Don't need to be complete.
  memset(isFollow,0,sizeof(isFollow));

  isFollow[SymbolType::StmtList][TokenType::RBRACE] = true;

}
//declarations of parse functions. 
// for example, p_VarDecl() recursively parse a VarDecl symbol and return a `VarDecl` node if success. 
static ast::Program* p_Program();
static ast::FuncDefn* p_Function();
static ast::Type* p_Type();
static ast::StmtList* p_StmtList();
static ast::Statement* p_Statement();
static ast::VarDecl* p_VarDecl();
static ast::ReturnStmt* p_Return();
static ast::CompStmt* p_Block();
static ast::IfStmt* p_If();
static ast::Expr* p_Expression();
static ast::Expr* p_Assignment();
static ast::Expr* p_Conditional();
static ast::Expr* p_LogicalOr();
static ast::Expr* p_LogicalAnd();
static ast::Expr* p_Equality();
static ast::Expr* p_Relational();
static ast::Expr* p_Additive();
static ast::Expr* p_Multiplicative();
static ast::Expr* p_Unary();
static ast::Expr* p_Primary();

static Token next_token;
//the next token to be consumed by the parser, also the lookahead token
//you can access next_token.type directly without calling lookahead(), thus not consuming the token.
//notice when to consume and when not to consume the token.


//parser entry point
ast::Program* MindCompiler::parseFile(const char* filename) {  
  ast::Program* ptree;
  
  init_first_set();
  init_follow_set();
  set_scanner_input(filename);
  next_token = yylex(); //prepare the first token
  ptree = p_Program();
  if(next_token.type!=TokenType::END){
    mind::err::issue(next_token.loc,new mind::err::SyntaxError("extra garbage at the end of file"));
  }
  close_scanner_input();
  return ptree;
}




Token lookahead(TokenType t){ 
  //consume one token with TokenType t(throw error if failed), and get the next token from scanner
  //to check the type of next_token without consuming it, access next_token.type directly
  Token tmp = next_token; //next_token: global variable declared before parseFile()
  if(next_token.type == t){
    next_token = yylex();
    return tmp;
  }
  mind::err::issue(next_token.loc,new mind::err::SyntaxError("expect " + TokenName[t] + " get " + TokenName[next_token.type]));
  return tmp;
}
Token lookahead(){ //when called without specifying a TokenType, we just jump the current token and get the next token
  Token tmp = next_token;
  next_token = yylex();
  return tmp;
}



static ast::Program* p_Program(){ // p: parse
  /* Program: FuncDefn */
  ast::FuncDefn* main_func =  p_Function();

  return new ast::Program(main_func, main_func->getLocation());
}

static ast::FuncDefn* p_Function(){
  /*  FuncDefn : Type Identifier '(' ')' '{'  StmtList  '}'  */

  ast::Type* return_type = p_Type();
  Token name = lookahead(TokenType::IDENTIFIER);
  lookahead(TokenType::LPAREN);
  lookahead(TokenType::RPAREN);
  lookahead(TokenType::LBRACE);
  ast::StmtList* statements = p_StmtList();
  lookahead(TokenType::RBRACE);
  return new ast::FuncDefn(name.id, return_type, new ast::VarList(), statements, return_type->getLocation());
}

static ast::Type* p_Type(){
  /*  type : Int  */
  
  // TODO:
  // 1. Match token `Int` using lookadhead(TokenType t).
  // 2. Build a `IntType` node and return it. Refer to ast/ast.hpp for detailed information.
}

static ast::StmtList* p_StmtList(){
  // StmtList : (Statement | VarDecl ';')*
  ast::StmtList* statements = new ast::StmtList();
  if (!isFirst[SymbolType::Statement][next_token.type] && 
    !isFirst[SymbolType::VarDecl][next_token.type] && 
    !isFollow[SymbolType::StmtList][next_token.type]) {
    mind::err::issue(next_token.loc, new mind::err::SyntaxError("unexpected " + TokenName[next_token.type]));
    lookahead();
  }
  while(!isFollow[SymbolType::StmtList][next_token.type]){
    if(isFirst[SymbolType::Statement][next_token.type]){
      // TODO: Complete the action if the next is a statement.
      // Using statements->append(...)
    }else if(isFirst[SymbolType::VarDecl][next_token.type]){
      // TODO: Complete the action if the next is a declaration. Remeber to consume ';'.
    }else{
      mind::err::issue(next_token.loc, new mind::err::SyntaxError("expect statement or vardecl, get" + TokenName[next_token.type]));
    }
  }
  return statements;
}

static ast::CompStmt* p_Block(){
  /*  block : '{' StmtList '}'  */
  Token lbrace = lookahead(TokenType::LBRACE);
  ast::StmtList* stmts = p_StmtList();
  lookahead(TokenType::RBRACE);
  return new ast::CompStmt(stmts, lbrace.loc);
}

static ast::Statement* p_Statement(){
  /*  statement : if | return | ( expression )? ';' | '{' StmtList '} */
  if(isFirst[SymbolType::Expression][next_token.type]){
    ast::Expr* stmt_as_expr = p_Expression();
    lookahead(TokenType::SEMICOLON);
    return new ast::ExprStmt(stmt_as_expr,stmt_as_expr->getLocation());
  }else{
    if(next_token.type == TokenType::RETURN){
      return p_Return();
    }
    // TODO: Complete the action to do if the next token is `If`/`LBRACE`/`SEMICOLON`
    // else if (...) {} 
    // else if (...) {}
  }
  mind::err::issue(next_token.loc, new mind::err::SyntaxError("expect statement, get" + TokenName[next_token.type]));
  return NULL;
}

static ast::VarDecl* p_VarDecl(){
  /* declaration : type Identifier ('=' expression)? */
  ast::Type* type = p_Type();
  Token identifier = lookahead(TokenType::IDENTIFIER);
  if(next_token.type == TokenType::ASSIGN){
    // TODO:
    /** 
     * 1. Match token `Assign`.
     * 2. Parse expression to get the initial value.
     * 3. Set the `init` of VarDecl.
     * 4. Return the VarDecl with `init` part
     **/
  }
  return new ast::VarDecl(identifier.id,type,type->getLocation());
}

static ast::ReturnStmt* p_Return(){
  /*  return : 'return' expression ';'  */

  // TODO:
  /**
   * 1. Match token 'Return'.
   * 2. Parse expression.
   * 3. Match token 'Semi'.
   * 4. Build a 'Return' node and return it.
   **/

}

static ast::IfStmt* p_If(){
  /*  if : 'if' '(' expression ')' statement ( 'else' statement )?  */

  // TODO:
  /**
   * 1. Match token 'If' and 'LParen'.
   * 2. Parse expression to get condition.
   * 3. Match token 'RParen'.
   * 4. Parse statement to get body.
   * 5. Build a 'If' node with condition and body.
   * 6. If the next token is 'Else', match token 'else' and parse statement to get otherwise of the node.
   * 7. Return the 'If' node.
   * 
   * hint: use an `EmptyStmt` for an empty else branch instead of NULL
   * hint: to check the type of next_token without consuming it, access next_token.type directly
   **/

}

static ast::Expr* p_Expression(){
  /* expression : assignment */

  // TODO:
  // 1. Parse assignment and return it.
}

static ast::Expr* p_Assignment(){
  /*    assignment : Identifier '=' expression
                    | conditional              */
  ast::Expr* node = p_Conditional(); 
  if(next_token.type == TokenType::ASSIGN){
  // TODO
  /**
   * 1. Match token 'Assign' , a.k.a. '='
   * 2. Parse expression to get rhs.
   * 3. Build an 'AssignExpr' node with node and rhs
   * 4. Return the node.
   * 
   * Hint: `AssignStmt` node need a `VarRef` node as its child, 
   * but p_Conditional() returns a `LValueExpr` node here,
   * hence some conversion is needed (`Expr` to `LvalueExpr` to `VarRef`)  
   * condider dynamic_cast<> here
   **/
  }
  return node;
}

static ast::Expr* p_Conditional(){
  /*  conditional : logical_or '?' expression ':' conditional
                  | logical_or                     */

  ast::Expr* node = p_LogicalOr();
  if(next_token.type==TokenType::QUESTION){
    Token question = lookahead();
    ast::Expr* then = p_Expression();
    lookahead(TokenType::COLON);
    ast::Expr* otherwise = p_Conditional();
    node = new ast::IfExpr(node,then,otherwise,question.loc);
  }

  return node;
  
}
static ast::Expr* p_LogicalOr(){
  /* logical_or : logical_or '||' logical_and
                | logical_and                */

  // equivalent EBNF:

  /* logical_or : logical_and { '||' logical_and }  */

  ast::Expr* node = p_LogicalAnd();

  while(next_token.type == TokenType::OR){
    Token Or = lookahead();
    ast::Expr* operand2 = p_LogicalAnd();
    node = new ast::OrExpr(node,operand2,Or.loc);
  }

  return node;
}

static ast::Expr* p_LogicalAnd(){
  /* logical_and : logical_and '&&' equality
                 | equality                  */

  // equivalent EBNF:

  /* logical_and : equality { '&&' equality }  */

  // TODO:
  // 1. Refer to the implementation of 'p_LogicalOr'. 
}

static ast::Expr* p_Equality(){
  /*     equality : equality '==' relational
                | equality '!=' relational
                | relational                      */

  // equivalent EBNF: 

  /* equality : relational { '==' relational | '!=' relational } */

  ast::Expr* node = p_Relational();
  while(next_token.type == TokenType::EQU || next_token.type == TokenType::NEQ){
 
    Token operation = lookahead();
    ast::Expr* operand2 = p_Relational(); 
 
    switch(operation.type){
      case TokenType::EQU:
        node = new ast::EquExpr(node,operand2,operation.loc);
        break;
      case TokenType::NEQ:
        node = new ast::NeqExpr(node,operand2,operation.loc);
        break;
      default:
        break;
    }
  }
  return node;
}

static ast::Expr* p_Relational(){
  /*   relational : relational '<' additive
                  | relational '>' additive
                  | relational '<=' additive
                  | relational '>=' additive
                  | additive                          */

  // equivalent EBNF:
  /* relational : additive { '<' additive | '>' additive | '<=' additive | '>=' additive} */

  // TODO:
  // 1. Refer to the implementation of 'p_Additive'.
}

static ast::Expr* p_Additive(){
  /*  additive : additive '+' multiplicative
                 | additive '-' multiplicative
                 | multiplicative        */
  
  // equivalent EBNF:

  /* additive : multiplicative { '+' multiplicative | '-' multiplicative }  */

  ast::Expr* node = p_Multiplicative();
  while(next_token.type == TokenType::PLUS || next_token.type == TokenType::MINUS){
 
    Token operation = lookahead();
    ast::Expr* operand2 = p_Multiplicative(); 
 
    switch(operation.type){
      case TokenType::PLUS:
        node = new ast::AddExpr(node,operand2,operation.loc);
        break;
      case TokenType::MINUS:
        node = new ast::SubExpr(node,operand2,operation.loc);
        break;
      default:
        break;
    }
 
  }
  return node;
}
static ast::Expr* p_Multiplicative(){
/*   multiplicative : multiplicative '*' unary
                   | multiplicative '/' unary
                   | multiplicative '%' unary
                   | unary       */
  
  // equivalent EBNF:
  
  /* multiplicative : unary { '*' unary | '/' unary | '%' unary }  */

  ast::Expr* node = p_Unary();
  while(next_token.type == TokenType::MODULO || next_token.type == TokenType::TIMES || next_token.type == TokenType::DIVIDE){
    
    Token operation = lookahead();
    ast::Expr* operand2 = p_Unary();

    switch(operation.type){
      case TokenType::MODULO:
        node = new ast::ModExpr(node,operand2,operation.loc);
        break;
      case TokenType::TIMES:
        node = new ast::MulExpr(node,operand2,operation.loc);
        break;
      case TokenType::DIVIDE:
        node = new ast::DivExpr(node,operand2,operation.loc);
        break;
      default:
        break;
    }

  }
  return node;
}

static ast::Expr* p_Unary(){
/*      unary : Minus unary
              | BitNot unary
              | Not unary
              | primary           */
  if(isFirst[SymbolType::Primary][next_token.type]){//LPAREN,IDENTIFIER, integer constant
/*
 isFirst[SvmbolType::Primary][next.token.type]用来判断下一个token 是否在 FIRST(primary)中,以选择产生式unary:primary。
在同学们需要填空的代码中，待选择的多个产生式右端通常都以终结符开始，因此只需通过判断 next token 的终结符类型选择产生式即可，
无需用到isFirst集合。同时请注意【目前的isFirst集合并不完整】，如果想使用，请按照自己需求【初始化对应项的值】。
*/
    return p_Primary();//recursively parse a Primary symbol
  }else{
     if(next_token.type == TokenType::MINUS){//negative -
       Token minus = lookahead(TokenType::MINUS);
       return new ast::NegExpr(p_Unary(),minus.loc);//p_Unary: recursively parse a Unary symbol
     }else if(next_token.type == TokenType::BNOT){ //bitwise not ~
       Token bnot = lookahead(TokenType::BNOT);
       return new ast::BitNotExpr(p_Unary(),bnot.loc);
     }else if(next_token.type == TokenType::LNOT){ //logical not !
       Token lnot = lookahead(TokenType::LNOT);
       return new ast::NotExpr(p_Unary(),lnot.loc);
     }   
  }
  mind::err::issue(next_token.loc, new mind::err::SyntaxError("expect unary expression get" + TokenName[next_token.type]));
  return NULL;//not to be execueted, only write this to eliminate compiler warnings.
}

static ast::Expr* p_Primary(){
  /*  primary : Integer
             | Identifier
             | '(' expression ')'   */
  if(next_token.type == TokenType::ICONST){
    Token iconst = lookahead(TokenType::ICONST);
    return new ast::IntConst(iconst.value,iconst.loc);
  }else if(next_token.type == TokenType::LPAREN){
    lookahead(TokenType::LPAREN);
    ast::Expr* expr = p_Expression();
    lookahead(TokenType::RPAREN);
    return expr;
  }else if(next_token.type == TokenType::IDENTIFIER){
    Token name = lookahead(TokenType::IDENTIFIER);
    return new ast::LvalueExpr(new ast::VarRef(name.id,name.loc),name.loc);
  }
  mind::err::issue(next_token.loc, new mind::err::SyntaxError("expect primary expression get" + TokenName[next_token.type]));
  return NULL;
}

