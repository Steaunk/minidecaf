/*****************************************************
 *  Implementation of "MindCompiler" class
 *
 *  Keltin Leung 
 */

#include "compiler.hpp"
#include "asm/riscv_md.hpp"
#include "ast/ast.hpp"
#include "config.hpp"
#include "options.hpp"
#include "scope/scope.hpp"
#include "scope/scope_stack.hpp"
#include "tac/tac.hpp"

#include "tac/flow_graph.hpp"

using namespace mind;
using namespace mind::assembly;

#include <fstream>
#include <iostream>

/* Constructor.
 */
MindCompiler::MindCompiler() {

    switch (Option::getArch()) {
    case Option::RISCV:
        md = new RiscvDesc();
        break;

    case Option::X86:
    case Option::PPC:
    case Option::MIPS:
        // currently, we don't support architectures other than RISC-V.
        // you could implement this as you extension.
        mind_assert(false);
        break;

    default:
        mind_assert(false);
    }
}

/* Compiles the input file into the output file.
 *
 * PARAMETERS:
 *   input  - the input file name (stdin if NULL)
 *   result - the output stream
 * EXCEPTIONS:
 *   if any errors occur, the function will not return.
 */
void MindCompiler::compile(const char *input, std::ostream &result) {
    // syntatical analysis
    ast::Program *tree = parseFile(input);
    // Checkpoint 1: if we get a bad AST, terminate the compilation.
    err::checkPoint();

    if (Option::getLevel() == Option::PARSER) {
        result << tree << std::endl;
        result.flush();
        return;
    }

    // semantical analysis
    buildSymbols(tree);
    // TO STUDENTS: if you want to have a look at the symbol tables,
    //              enable the following 2 lines
    // result << tree->ATTR(gscope) << std::endl;
    // result.flush();

    // Checkpoint 2: if we get bad symbol tables, terminate the compilation.
    err::checkPoint();

    checkTypes(tree);

    // Checkpoint 3: if the typing rules were not satisfied, terminate the
    // program.
    err::checkPoint();

    if (Option::getLevel() == Option::SEMANTIC) {
        result << tree->ATTR(gscope) << std::endl;
        result.flush();
        return;
    }

    // translating to linear IR
    tac::Piece *ir = translate(tree);

    if (Option::getLevel() == Option::TACGEN) {
        ir->dump(result);
        result << std::endl;
        result.flush();
        return;
    }

    // translating to assembly code (now let's go to MipsDesc::emitPieces)
    md->emitPieces(tree->ATTR(gscope), ir, result);

    // now we are done! thank you for your participation in the Mind project.
}
