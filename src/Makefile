######################################################
#  GNU Make File for Mind Compiler
#
#
#  NOTE: Please adjust me and run "make depend" after
#        adding new files.
#

.SUFFIXES:
.SUFFIXES: .cpp .o

LEX = flex
YACC = bison
DEFINES =
CXX = g++
CC = gcc
INCLUDES = -I .
CFLAGS = $(INCLUDES) $(DEFINES)  -g -Wall -pipe -DUSING_GCC
CXXFLAGS = $(INCLUDES) $(DEFINES)  -g -Wall -pipe -DUSING_GCC
YFLAGS = -dv
LDFLAGS = -lm -lgc -g
CSH = bash

# lex.yy.c is usually compiled with -O to speed it up.

LEXCFLAGS = -O $(CFLAGS)

SCANNER = scanner.cpp
PARSER  = parser.cpp parser.hpp location.hh position.hh stack.hh
AST     = ast/ast.o ast/ast_add_expr.o ast/ast_and_expr.o  ast/ast_or_expr.o\
          ast/ast_assign_expr.o ast/ast_bool_const.o \
          ast/ast_bool_type.o ast/ast_bitnot_expr.o \
          ast/ast_cmp_expr.o ast/ast_div_expr.o ast/ast_equ_expr.o ast/ast_neq_expr.o ast/ast_expr_stmt.o \
          ast/ast_if_stmt.o ast/ast_int_const.o ast/ast_int_type.o \
          ast/ast_lvalue_expr.o ast/ast_mod_expr.o \
          ast/ast_mul_expr.o ast/ast_neg_expr.o \
          ast/ast_not_expr.o \
          ast/ast_program.o ast/ast_func_defn.o \
          ast/ast_return_stmt.o ast/ast_sub_expr.o \
          ast/ast_var_decl.o ast/ast_var_ref.o  ast/ast_while_stmt.o ast/ast_comp_stmt.o ast/ast_index_expr.o 
TYPE    = type/type.o type/base_type.o type/array_type.o type/func_type.o
SYMTAB  = symb/symbol.o symb/variable.o symb/function.o
SCOPE   = scope/scope_stack.o scope/scope.o \
          scope/global_scope.o scope/func_scope.o scope/local_scope.o
TAC     = tac/tac.o tac/trans_helper.o tac/flow_graph.o
ASM     = asm/offset_counter.o asm/riscv_md.o asm/riscv_frame_manager.o
FRONTEND = scanner.o parser.o
TRANSLATION     = translation/translation.o translation/build_sym.o translation/type_check.o
DATAFLOW = tac/dataflow.o
OBJS    = main.o compiler.o \
	  options.o error.o misc.o \
          $(AST) $(TYPE) $(SYMTAB) $(SCOPE) $(TAC) $(ASM) \
	  $(FRONTEND) $(TRANSLATION) $(DATAFLOW)


all:	$(SCANNER) $(PARSER) $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o mind

$(SCANNER): frontend/scanner.l
	$(LEX) $(LFLAGS) $<

$(PARSER): frontend/parser.y
	$(YACC) $(YFLAGS) $<

clean:
	rm -f mind *.o *.output $(SCANNER) $(PARSER) $(OBJS)

#
# DO NOT DELETE THIS LINE -- make depend depends on it.


compiler.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
compiler.o: error.hpp ast/ast.hpp scope/scope.hpp scope/scope_stack.hpp
compiler.o: 3rdparty/stack.hpp tac/tac.hpp 3rdparty/set.hpp asm/riscv_md.hpp
compiler.o: asm/mach_desc.hpp asm/riscv_frame_manager.hpp compiler.hpp
compiler.o: options.hpp tac/flow_graph.hpp 3rdparty/vector.hpp
error.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
error.o: error.hpp symb/symbol.hpp type/type.hpp scope/scope.hpp location.hpp
error.o: errorbuf.hpp
main.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
main.o: error.hpp compiler.hpp options.hpp
misc.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
misc.o: error.hpp location.hpp
options.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
options.o: error.hpp options.hpp
parser.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
parser.o: error.hpp ast/ast.hpp location.hpp compiler.hpp
scanner.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
scanner.o: error.hpp ast/ast.hpp parser.hpp location.hpp
ast/ast.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
ast/ast.o: error.hpp ast/ast.hpp options.hpp location.hpp
ast/ast_add_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_add_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_and_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_and_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_assign_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_assign_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_bitnot_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_bitnot_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_bool_const.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_bool_const.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_bool_type.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_bool_type.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_cmp_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_cmp_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_while_stmt.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_while_stmt.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_comp_stmt.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_comp_stmt.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_div_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_div_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_equ_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_equ_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_expr_stmt.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_expr_stmt.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_func_defn.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_func_defn.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_if_stmt.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_if_stmt.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_int_const.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_int_const.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_int_type.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_int_type.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_lvalue_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_lvalue_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_mod_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_mod_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_mul_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_mul_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_neg_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_neg_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_neq_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_neq_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_not_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_not_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_or_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_or_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_program.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_program.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_return_stmt.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_return_stmt.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_sub_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_sub_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_var_decl.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_var_decl.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_var_ref.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_var_ref.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
ast/ast_index_expr.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
ast/ast_index_expr.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
tac/flow_graph.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
tac/flow_graph.o: 3rdparty/list.hpp error.hpp tac/tac.hpp 3rdparty/set.hpp
tac/flow_graph.o: tac/flow_graph.hpp 3rdparty/vector.hpp asm/mach_desc.hpp
tac/flow_graph.o: 3rdparty/map.hpp
tac/tac.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
tac/tac.o: error.hpp tac/tac.hpp 3rdparty/set.hpp tac/flow_graph.hpp
tac/tac.o: 3rdparty/vector.hpp asm/mach_desc.hpp options.hpp
tac/trans_helper.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
tac/trans_helper.o: 3rdparty/list.hpp error.hpp tac/tac.hpp 3rdparty/set.hpp
tac/trans_helper.o: tac/trans_helper.hpp symb/symbol.hpp type/type.hpp
tac/trans_helper.o: scope/scope.hpp scope/scope_stack.hpp 3rdparty/stack.hpp
tac/trans_helper.o: asm/mach_desc.hpp asm/offset_counter.hpp
symb/function.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
symb/function.o: 3rdparty/list.hpp error.hpp symb/symbol.hpp type/type.hpp
symb/function.o: scope/scope.hpp scope/scope_stack.hpp 3rdparty/stack.hpp
symb/function.o: tac/tac.hpp 3rdparty/set.hpp
symb/symbol.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
symb/symbol.o: error.hpp symb/symbol.hpp type/type.hpp scope/scope.hpp
symb/variable.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
symb/variable.o: 3rdparty/list.hpp error.hpp symb/symbol.hpp type/type.hpp
symb/variable.o: scope/scope.hpp
type/array_type.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
type/array_type.o: 3rdparty/list.hpp error.hpp type/type.hpp
type/base_type.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
type/base_type.o: 3rdparty/list.hpp error.hpp type/type.hpp
type/func_type.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
type/func_type.o: 3rdparty/list.hpp error.hpp type/type.hpp
type/type.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
type/type.o: error.hpp type/type.hpp
scope/func_scope.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
scope/func_scope.o: 3rdparty/list.hpp error.hpp scope/scope.hpp symb/symbol.hpp
scope/func_scope.o: type/type.hpp 3rdparty/vector.hpp
scope/global_scope.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
scope/global_scope.o: 3rdparty/list.hpp error.hpp scope/scope.hpp
scope/global_scope.o: symb/symbol.hpp type/type.hpp 3rdparty/vector.hpp
scope/local_scope.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
scope/local_scope.o: 3rdparty/list.hpp error.hpp scope/scope.hpp symb/symbol.hpp
scope/local_scope.o: type/type.hpp 3rdparty/vector.hpp
scope/scope.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
scope/scope.o: error.hpp scope/scope.hpp symb/symbol.hpp type/type.hpp
scope/scope.o: location.hpp
scope/scope_stack.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
scope/scope_stack.o: 3rdparty/list.hpp error.hpp scope/scope_stack.hpp
scope/scope_stack.o: scope/scope.hpp 3rdparty/stack.hpp
asm/offset_counter.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
asm/offset_counter.o: 3rdparty/list.hpp error.hpp asm/offset_counter.hpp
translation/build_sym.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
translation/build_sym.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
translation/build_sym.o: scope/scope.hpp scope/scope_stack.hpp 3rdparty/stack.hpp
translation/build_sym.o: symb/symbol.hpp type/type.hpp compiler.hpp
translation/type_check.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
translation/type_check.o: 3rdparty/list.hpp error.hpp ast/ast.hpp ast/visitor.hpp
translation/type_check.o: type/type.hpp scope/scope_stack.hpp scope/scope.hpp
translation/type_check.o: 3rdparty/stack.hpp symb/symbol.hpp compiler.hpp
translation/translation.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
translation/translation.o: 3rdparty/list.hpp error.hpp ast/ast.hpp symb/symbol.hpp
translation/translation.o: type/type.hpp scope/scope.hpp tac/trans_helper.hpp
translation/translation.o: tac/tac.hpp 3rdparty/set.hpp translation/translation.hpp
translation/translation.o: ast/visitor.hpp 3rdparty/vector.hpp compiler.hpp asm/offset_counter.hpp
tac/dataflow.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
tac/dataflow.o: error.hpp tac/tac.hpp 3rdparty/set.hpp tac/flow_graph.hpp
tac/dataflow.o: 3rdparty/vector.hpp asm/mach_desc.hpp
asm/riscv_frame_manager.o: config.hpp 3rdparty/boehmgc.hpp define.hpp
asm/riscv_frame_manager.o: 3rdparty/list.hpp error.hpp tac/tac.hpp 3rdparty/set.hpp
asm/riscv_frame_manager.o: asm/riscv_frame_manager.hpp
asm/riscv_md.o: config.hpp 3rdparty/boehmgc.hpp define.hpp 3rdparty/list.hpp
asm/riscv_md.o: error.hpp scope/scope.hpp symb/symbol.hpp type/type.hpp
asm/riscv_md.o: asm/riscv_md.hpp 3rdparty/set.hpp asm/mach_desc.hpp
asm/riscv_md.o: asm/riscv_frame_manager.hpp asm/offset_counter.hpp
asm/riscv_md.o: tac/tac.hpp tac/flow_graph.hpp 3rdparty/vector.hpp options.hpp
