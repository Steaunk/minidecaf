#ifndef __MIND_MYPARSER__
#define __MIND_MYPARSER__

#include "config.hpp"
#include "ast/ast.hpp"
#include "location.hpp"
#include "compiler.hpp"
#include "error.hpp"
#include <string>
using namespace mind;
enum TokenType{
    IDENTIFIER,
    INT,ICONST,   //INT : "int" ICONST: "123" , "0"
    RETURN,
    LPAREN,RPAREN,
    LBRACE,RBRACE,
    SEMICOLON,ASSIGN,
    QUESTION,COLON,
    AND,OR,
    LT,GT,
    LEQ,GEQ,
    EQU,NEQ,
    PLUS,MINUS,
    TIMES,DIVIDE,MODULO, //  + - * / %
    BNOT,LNOT,//~,!
    IF,ELSE,
    END //end of file
};
enum SymbolType{
    Program,
    FuncDefn,
    StmtList,
    Statement,
    VarDecl,
    Primary,
    Unary,
    Multiplicative,
    Additive,
    Relational,
    Equality,
    LogicalAnd,
    LogicalOr,
    Conditional,
    Assignment,
    Expression
};//slightly different from ASTNode::NodeType
class Token{
public:
    TokenType type;
    int value; //ICONST
    std::string id;//IDENTIFIER
    Location* loc;
    Token(){}
    Token(TokenType t,Location* l){
        type = t;
        value = 0;
        id = "";
        loc = l;
    }
    Token(TokenType t,int v,Location* l){
        type = t;
        value = v;
        id = "";
        loc = l;

    }
    Token(TokenType t, std::string name, Location* l){
        type = t;
        value = 0;
        id = name;
        loc = l;
    }
};
#endif