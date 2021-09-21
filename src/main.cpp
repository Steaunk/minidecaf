/*****************************************************
 *  The Main Entry.
 *
 *
 *  Keltin Leung 
 */

#include "compiler.hpp"
#include "config.hpp"
#include "options.hpp"

#include <fstream>

using namespace mind;

/* The main entry of the program.
 *
 * PARAMETERS:
 *   argc   - the argument count (i.e. length of argv[])
 *   argv   - the argument list (this is an array of string)
 */
int main(int argc, char **argv) {
    // enables BoehmGC
    GC_INIT();

    // parses the command line options (SEE ALSO: mind::Option)
    Option::parse(argc, argv);
    // creates an instance of the compiler
    MindCompiler *c = new MindCompiler();
    // let's go!
    if (Option::getOutput() == NULL) {
        c->compile(Option::getInput(), std::cout);
        std::cout.flush();
    } else {
        std::ofstream fout(Option::getOutput());
        c->compile(Option::getInput(), fout);
        fout.flush();
        fout.close();
    }

    return 0;
}
