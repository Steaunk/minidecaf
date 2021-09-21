/*****************************************************
 *  Definition of Command Line Options.
 *
 *  If you'd like to see the option description, please
 *  look for "showUsage()" function in options.cpp.
 *
 *  Keltin Leung 
 */

#ifndef __MIND_OPTIONS__
#define __MIND_OPTIONS__

namespace mind {

/* Command line options.
 */
class Option {
  public:
    /* Option values */
    typedef enum {
        UNKNOWN,
        PARSER,
        SEMANTIC,
        TACGEN,
        DATAFLOW,
        ASMGEN,
        MIPS,
        RISCV,
        X86,
        PPC
    } opt_t;

    static opt_t getLevel(void);  // Gets the current developing level
    static opt_t getArch(void);   // Gets the target architecture
    static bool doOptimize(void); // Gets whether optimization will be done
    static const char *getInput(void);
    static const char *getOutput(void);
    static void parse(int argc, char **argv); // Parses the command line

  private:
    static opt_t level;        // Current developing level
    static opt_t arch;         // Target architecture
    static bool optimize;      // Whether optimization will be done
    static const char *input;  // Input file name
    static const char *output; // Output file name

    Option() { /* do not instantiate me */
    }
};

} // namespace mind

#endif // __MIND_OPTIONS__
