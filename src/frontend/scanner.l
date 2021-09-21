/*****************************************************
 *  The GNU Flex Specification for Mind Language.
 *
 *  We have provided complete SECTION I & IV for you.
 *  Please complete SECTION II & III.
 *
 *  In case you want some debug support, we provide a
 *  "diagnose()" function for you. All you need is to 
 *  call this function in main.cpp.
 *
 *  Please refer to the ``GNU Flex Manual'' if you have
 *  problems about how to write the lexical rules.
 *
 *  Keltin Leung 
%option yylineno noyywrap nounistd nounput bison-locations never-interactive  noinput batch debug
 */
%option yylineno noyywrap nounput noinput batch

%option outfile="scanner.cpp"
/* %option outfile="scanner.cpp" header-file="scanner.hpp" */

/* SECTION I: preamble inclusion. */
%{
#include "config.hpp"
#include "ast/ast.hpp"
#include "parser.hpp"
using namespace mind;
#include "location.hpp"

#include <cstdlib>
#include <iostream>
#include <climits>

using namespace mind::err;
yy::location loc;
# define YY_DECL \
  yy::parser::symbol_type yylex ()
// ... and declare it for the parser's sake.
YY_DECL;

%}

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_ICONST (const std::string &s, const yy::parser::location_type& loc);
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
{NEWLINE}     { loc.lines (yyleng); loc.step (); }
"//"          { BEGIN(C);         }
<C>{NEWLINE}  { BEGIN(0); loc.lines (yyleng); loc.step ();        }
<C><EOF>      { return yy::parser::make_END (loc); }
<C>.          { /* ignore */      }

"/*"          { BEGIN(B); }
<B>"*/"       { BEGIN(0); }
<B>.|\n ;  
<B><EOF> { return yy::parser::make_END (loc); }
<<EOF>> { return yy::parser::make_END (loc); }

"int"         { return yy::parser::make_INT  (loc);      }
"return"      { return yy::parser::make_RETURN  (loc);   }
";"          { return yy::parser::make_SEMICOLON  (loc); }    
"("          { return yy::parser::make_LPAREN  (loc);       }
")"          { return yy::parser::make_RPAREN  (loc);       }
"{"          { return yy::parser::make_LBRACE  (loc);       }
"}"          { return yy::parser::make_RBRACE  (loc);       }
"+"          { return yy::parser::make_PLUS  (loc);       }
"-"          { return yy::parser::make_MINUS  (loc);       }
"if"         { return yy::parser::make_IF  (loc);       }
"else"         { return yy::parser::make_ELSE  (loc);       }
"while"       { return yy::parser::make_WHILE  (loc);       }
"break"       { return yy::parser::make_BREAK (loc);       }

{INTEGER}     {return make_ICONST(yytext, loc);}

{IDENTIFIER}  {return yy::parser::make_IDENTIFIER (yytext, loc); }

.             { } 

%%
/* SECTION IV: customized section */
int myline, mycol;
yy::parser::symbol_type
make_ICONST (const std::string &s, const yy::parser::location_type& loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_ICONST ((int) n, loc);
}
void scan_begin(const char* filename){
  if (NULL == filename)
	yyin = stdin;
  else
	yyin = std::fopen(filename, "r");
}
void scan_end(){
   if (yyin != stdin)
	  std::fclose(yyin);
}