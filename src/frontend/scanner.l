%option yylineno noyywrap nounput noinput batch

%option outfile="scanner.cpp"

%{
#include "config.hpp"
#include "ast/ast.hpp"
#include "frontend/myparser.hpp"
using namespace mind;
#include "location.hpp"

#include <cstdlib>
#include <iostream>
#include <climits>

using namespace mind::err;
#define YY_USER_ACTION   updateLocation(loc);
Location loc(0);
inline void updateLocation(Location& loc);
Token make_ICONST (std::string s, Location* loc);
//yylex   
# define YY_DECL \
  Token yylex ()
// ... and declare it for the parser's sake.
YY_DECL;
%}

/* SECTION II: macro definition */
NEWLINE       (\r|\n|\r\n)
WHITESPACE    ([ \t]+)
INTEGER       ([0-9]+)
IDENTIFIER    ([A-Za-z][_0-9A-Za-z]*)

/* SECTION III: matching rules (and actions) */
%x B C
%%
{WHITESPACE}  {       }
{NEWLINE}     { loc.col = 0; }
"//"          { BEGIN(C);         }
<C>{NEWLINE}  { BEGIN(0);        }
<C><EOF>      { return Token(TokenType::END,new Location(loc)); }
<C>.          { /* ignore */      }

"/*"          { BEGIN(B); }
<B>"*/"       { BEGIN(0); }
<B>.|\n ;  
<B><EOF> { return Token(TokenType::END,new Location(loc)); }
<<EOF>> { return Token(TokenType::END,new Location(loc)); }

"int"         { return Token(TokenType::INT,new Location(loc));      }
"return"      { return Token(TokenType::RETURN,new Location(loc));   }
";"          { return Token(TokenType::SEMICOLON,new Location(loc)); }    
":"          { return Token(TokenType::COLON,new Location(loc)); }    
"("          { return Token(TokenType::LPAREN,new Location(loc));       }
")"          { return Token(TokenType::RPAREN,new Location(loc));      }
"{"          { return Token(TokenType::LBRACE,new Location(loc));       }
"}"          { return Token(TokenType::RBRACE,new Location(loc));     }
"~"          { return Token(TokenType::BNOT,new Location(loc));       }
"!"          { return Token(TokenType::LNOT,new Location(loc));       }
"?"          { return Token(TokenType::QUESTION,new Location(loc));       }
"="          { return Token(TokenType::ASSIGN,new Location(loc));       }
"=="          { return Token(TokenType::EQU,new Location(loc));       }
"!="          { return Token(TokenType::NEQ,new Location(loc));       }
"<="          { return Token(TokenType::LEQ,new Location(loc));       }
">="          { return Token(TokenType::GEQ,new Location(loc));       }
"<"          { return Token(TokenType::LT,new Location(loc));       }
">"          { return Token(TokenType::GT,new Location(loc));       }
"&&"          { return Token(TokenType::AND,new Location(loc));       }
"||"          { return Token(TokenType::OR,new Location(loc));       }

"+"          { return Token(TokenType::PLUS,new Location(loc));       }
"-"          { return Token(TokenType::MINUS,new Location(loc));       }
"*"          { return Token(TokenType::TIMES,new Location(loc));       }
"/"          { return Token(TokenType::DIVIDE,new Location(loc));       }
"%"          { return Token(TokenType::MODULO,new Location(loc));       }
"if"         { return Token(TokenType::IF,new Location(loc));       }
"else"         { return Token(TokenType::ELSE,new Location(loc));        }

{INTEGER}     {return make_ICONST(yytext,new Location(loc));}

{IDENTIFIER}  {return Token(TokenType::IDENTIFIER, yytext, new Location(loc)); }

.             { mind::err::issue(new Location(loc),new mind::err::UnrecogCharError(yytext[0])); } 
%%
/* SECTION IV: customized section */
inline void
updateLocation(Location &loc) {
  loc.line = yylineno;
  loc.col = loc.col + yyleng;
}

void set_scanner_input(const char* filename){
  if (NULL == filename)
	yyin = stdin;
  else
	yyin = std::fopen(filename, "r");
}
void close_scanner_input(){
   if (yyin != stdin)
	  std::fclose(yyin);
}
Token make_ICONST (std::string s, Location* loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    mind::err::issue(loc, new mind::err::IntTooLargeError());
  return Token (TokenType::ICONST, (int) n, loc);
}
