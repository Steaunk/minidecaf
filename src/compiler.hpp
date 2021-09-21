/*****************************************************
 *  MindCompiler class.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_COMPILER__
#define __MIND_COMPILER__

#include "define.hpp"
#include "parser.hpp"
#include <iostream>
#define YY_DECL yy::parser::symbol_type yylex()
// ... and declare it for the parser's sake.
YY_DECL;
namespace mind {

class MindCompiler {
  public:
    MindCompiler();
    void compile(const char *input, std::ostream &result);

    ast::Program *parseFile(const char *filename);
    void buildSymbols(ast::Program *tree);
    void checkTypes(ast::Program *tree);
    tac::Piece *translate(ast::Program *tree);

    virtual ~MindCompiler() {}

  private:
    assembly::MachineDesc *md; // machine description
};
} // namespace mind

#endif // __MIND_COMPILER__
