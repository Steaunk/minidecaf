/*****************************************************
 *  Implementation of "Option".
 *
 *  If you'd like to see the option description, please
 *  look for "showUsage()" function in options.cpp.
 *
 *  Keltin Leung 
 */

#include "options.hpp"
#include "config.hpp"

#include <cstring>
#include <iostream>

using namespace mind;

// The current running level (PARSER/SEMANTIC/TACGEN/DATAFLOW/ASMGEN)
Option::opt_t Option::level = UNKNOWN;

// The backend architecture
Option::opt_t Option::arch = UNKNOWN;

// The source file
const char *Option::input = NULL;

// The output file
const char *Option::output = NULL;

// Whether to do extra optimization
bool Option::optimize = false;

/* Gets the current developing level.
 *
 * RETURNS:
 *   the current developing level
 */
Option::opt_t Option::getLevel(void) { return level; }

/* Gets the target architecture.
 *
 * RETURNS:
 *   the selected target architecure
 */
Option::opt_t Option::getArch(void) { return arch; }

/* Gets whether optimization will be done.
 *
 * RETURNS:
 *   whether compiler optimization will be done
 */
bool Option::doOptimize(void) { return optimize; }

/* Gets the input file name.
 *
 * RETURNS:
 *   the input file name
 */
const char *Option::getInput(void) { return input; }

/* Gets the output file name.
 *
 * RETURNS:
 *   the output file name
 */
const char *Option::getOutput(void) { return output; }

/* Prints the usage of this program.
 *
 */
static void showUsage(void) {
    std::cout
        << std::endl
        << "Usage: mdc [-l LEVEL] [-m ARCH] [-o OUTPUT] [-O] SOURCE"
        << std::endl
        << "Options:" << std::endl
        << "  -l  Specifying the developing level, where LEVEL is one of:"
        << std::endl
        << "      1 (syntax analysis)" << std::endl
        << "      2 (semantic resolution)" << std::endl
        << "      3 (IR translation)" << std::endl
        << "      4 (IR optimization)" << std::endl
        << "      5 (code generation. DEFAULT)" << std::endl
        << "  -m  Specifying the target architecture, where ARCH is one of:"
        << std::endl
        << "      riscv(DEFAULT), mips, x86, ppc" << std::endl
        << "  -o  Specifying the name of the output file (DEFAULT: stdout)."
        << std::endl
        << "  -O  Turn on compiler optimization (DEFAULT: off)." << std::endl
        << "" << std::endl;
}

/* Parses the command line.
 *
 * RETURNS:
 *   parse the command line options
 */
void Option::parse(int argc, char **argv) {
    int i = 1;
    const char *str[] = {"?", "1",    "2",     "3",   "4",
                         "5", "mips", "riscv", "x86", "ppc"};

    while (i < argc) {
        if (strcmp(argv[i], "-l") == 0) {
            if (i + 1 >= argc)
                goto bad_option;
            else if (level != UNKNOWN)
                goto dup_option;

            ++i;
            for (int j = PARSER; j <= ASMGEN; ++j)
                if (strcmp(argv[i], str[j]) == 0)
                    level = (Option::opt_t)j;

            if (level == UNKNOWN)
                goto bad_option;

        } else if (strcmp(argv[i], "-m") == 0) {
            if (i + 1 >= argc)
                goto bad_option;
            else if (arch != UNKNOWN)
                goto dup_option;

            ++i;
            for (int j = MIPS; j <= PPC; ++j)
                if (strcmp(argv[i], str[j]) == 0)
                    arch = (Option::opt_t)j;

            if (arch == UNKNOWN)
                goto bad_option;

        } else if (strcmp(argv[i], "-o") == 0) {
            if (i >= argc)
                goto bad_option;
            else if (output != NULL)
                goto dup_option;

            ++i;

            output = argv[i];

        } else if (strcmp(argv[i], "-O") == 0) {
            optimize = true;

        } else if (argv[i][0] == '-') {
            std::cerr << "Unknown option: '" << argv[0] << "'" << std::endl;
            showUsage();
            exit(1);

        } else if (input != NULL) {
            std::cerr << "Multiple source files." << std::endl;
            exit(1);

        } else {
            input = argv[i];
        }

        i++;
    }

    // resolve the default values
    if (level == UNKNOWN)
        level = ASMGEN;

    if (arch == UNKNOWN)
        arch = RISCV;

    return;

dup_option:
    std::cout << "Duplicated option: " << argv[i] << std::endl;
    exit(1);

bad_option:
    std::cout << "Bad option: " << argv[i] << std::endl;
    showUsage();
    exit(1);
}
