# MiniDecaf-2021

##  C++实验框架

### 环境配置

需要安装的工具软件有 `flex`, `bison`, `bohemgc`。具体的安装方式在文档中也有说明，此处为了方便再附一份。

关于操作系统，助教推荐使用 Linux 环境（如 Ubuntu，Debain 或 Windows 下的 WSL 等），当然你也可以在类 Unix 系统环境（Mac OS）中进行开发。助教不推荐直接在 Window 中搭建开发环境。对于 C++ 实验框架，你需要安装或保证如下软件满足我们的要求：

1. Flex

   Flex 是一个自动生成词法分析器的工具，它生成的词法分析器可以和 Bison 生成的语法分析器配合使用。我们推荐从 [Github](https://github.com/westes/flex/releases) 下载安装最新版本(在 2021.9.1, 最新版本是 2.6.4,不推荐使用低于 2.6 的版本)。

   在 Ubuntu 下，`apt-get install flex` 安装的 Flex 版本为 2.6，是可用的。

   在 Mac OS 下，推荐使用 homebrew 进行安装，`brew install flex` 安装的 Flex 版本为 2.6，是可用的。

2. Bison

   Bison是一个自动生成语法分析器的工具,它生成的语法分析器可以和Flex生成的词法分析器配合使用。

   在 Ubuntu 下，我们推荐从[官网](http://ftp.gnu.org/gnu/bison/)下载安装最新版本（在2021.9.1, 最新版本是3.7.6，不推荐使用低于3.7的版本,如 Ubuntu apt-get install 安装的3.0.4版本是不可用的）。下载解压 tar.gz 文件后， 在路径下执行`./configure && make && make install`, 就应该能正确安装。如果发生失败，就尝试`sudo ./configure` `sudo make`, `sudo make install`, 然后`bison --version`检查一下版本是否为3.7.6就可以了。

   在 Mac OS 下，推荐使用 homebrew 进行安装，`brew install bison` 安装的 Bison 版本为 3.7.6，是可用的。

   > 如果你是 Mac OS 用户，需要注意的是，系统可能已经安装了低版本的 flex 与 bison，安装的新版本工具会被覆盖，需要通过以下命令确认一下二者的版本：

   ```bash
   $ flex --version
   $ bison --version
   ```

   如果版本较低，需要将新安装的工具路径加入环境变量，关于路径，在**助教的电脑**上是：

   ```
   Flex: /usr/local/Cellar/flex/2.6.4_2/bin
   Bison: /usr/local/Cellar/bison/3.7.6/bin
   ```

3. Boehmgc

   C++ 语言的实验框架中，为了简化内存分配的处理，使用了一个第三方垃圾回收库，简单来说，使用这个垃圾回收库提供垃圾回收功能后，我们在框架里可以new了之后不用delete也不会出问题。

   在 Ubuntu 下，通过  `apt-get install libgc-dev ` 安装的 boehmgc 库是可用的。

   在 Mac OS 下，通过 `brew install libgc`  安装的 boehmgc 库是可用的。

4. 助教推荐的 gcc 版本为 8.5.0。

   > 需要注意的是，如果你使用 Mac OS 进行开发，Mac 自带的 g++ 命令极有可能软链接到了 clang，我们的实验框架在某些版本的 clang 下无法编译通过，因此推荐你使用如下方法安装特定版本的 gcc。安装完成之后，你需要使用 gcc-8，g++-8 来调用特定版本的 gcc，g++，同时你需要修改我们提供的 Makefile 中的 CC 与 CXX 选项。
   >
   > 另外，由于你使用了自己安装的 g++-8 编译程序，你需要将 boehmgc 的路径加入 g++ 的环境变量：

   ```bash
   # 你需要将以下内容加入你终端的配置文件（zsh->.zshrc; bash->.bash_profile)
   export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/usr/local/include"
   export LIBRARY_PATH="$LIBRARY_PATH:/usr/local/lib"
   export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib
   ```

   ```bash
   # Mac OS
   $ brew install gcc@8
   # Ubuntu
   $ sudo apt-get install gcc-8
   ```

> 需要说明的是，C++ 的环境配置较为复杂，教程可能无法覆盖到所有问题，欢迎随时联系助教。

### 运行方法

实验框架中为你准备好了 Makefile，你只需使用给定的 Makefile 就可以完成项目的构建，具体的编译和运行方式见下：

> 如果你更新和项目结构，比如增加了一个文件，那么就需要修改 Makefile 以符合你的要求。

```shell
# 编译
$ cd src
$ make
... 此时编译好的可执行文件（也就是编译器程序）名为mind
# 执行
# ./mind -l <level> <source file>
# 其中<level>有<1-5>可选，分别对应：
# 1-输出语法树；2-输出语义分析之后的符号表；3-输出中间代码；4-输出数据流分析结果；5-输出目标汇编代码
# 例如，我想输出某个程序的汇编代码到标准输出，只需要执行(默认输出文件名为input.c)
$ ./mind -l 5 input.c
# 当然，你也可以指定后端平台(risc-v,mips等)，只不过目前框架缺省平台为risc-v，且只支持risc-v
$ ./mind -l 5 -m riscv input.c
```

### 项目结构

```
MiniDecaf/src
├── 3rdparty----------------------------# 第三方文件夹
|  ├── boehmgc.hpp
|  ├── hash.hpp
|  ├── hash_fun.hpp
|  ├── hash_iterator.hpp
|  ├── hash_map.hpp
|  ├── hash_table.hpp
|  ├── list.hpp
|  ├── map.hpp
|  ├── set.hpp
|  ├── stack.hpp
|  └── vector.hpp
├── asm---------------------------------# 汇编代码生成模块
|  ├── mach_desc.hpp					
|  ├── offset_counter.cpp
|  ├── offset_counter.hpp
|  ├── riscv_frame_manager.cpp
|  ├── riscv_frame_manager.hpp
|  ├── riscv_md.cpp
|  └── riscv_md.hpp
├── ast---------------------------------# 抽象语法树节点定义
|  ├── ast.cpp
|  ├── ast.hpp
|  ├── ast_add_expr.cpp
|  ├── ast_and_expr.cpp
|  ├── ast_assign_expr.cpp
|  ├── ast_bitnot_expr.cpp
|  ├── ast_bool_const.cpp
|  ├── ast_bool_type.cpp
|  ├── ast_cmp_expr.cpp
|  ├── ast_comp_stmt.cpp
|  ├── ast_comp_stmt.o
|  ├── ast_div_expr.cpp
|  ├── ast_equ_expr.cpp
|  ├── ast_expr_stmt.cpp
|  ├── ast_func_defn.cpp
|  ├── ast_if_stmt.cpp
|  ├── ast_int_const.cpp
|  ├── ast_int_type.cpp
|  ├── ast_lvalue_expr.cpp
|  ├── ast_mod_expr.cpp
|  ├── ast_mul_expr.cpp
|  ├── ast_neg_expr.cpp
|  ├── ast_neq_expr.cpp
|  ├── ast_not_expr.cpp
|  ├── ast_or_expr.cpp
|  ├── ast_program.cpp
|  ├── ast_return_stmt.cpp
|  ├── ast_sub_expr.cpp
|  ├── ast_var_decl.cpp
|  ├── ast_var_ref.cpp
|  ├── ast_while_stmt.cpp
|  └── visitor.hpp
├── frontend----------------------------# 词法与语法规则定义
|  ├── parser.y
|  └── scanner.l
├── scope-------------------------------# 作用域模块
|  ├── func_scope.cpp
|  ├── global_scope.cpp
|  ├── local_scope.cpp
|  ├── scope.cpp
|  ├── scope.hpp
|  ├── scope_stack.cpp
|  └── scope_stack.hpp
├── symb--------------------------------# 符号表定义模块
|  ├── function.cpp
|  ├── symbol.cpp
|  ├── symbol.hpp
|  └── variable.cpp
├── tac---------------------------------# 三地址码定义、数据流图模块
|  ├── dataflow.cpp
|  ├── flow_graph.cpp
|  ├── flow_graph.hpp
|  ├── tac.cpp
|  ├── tac.hpp
|  ├── trans_helper.cpp
|  └── trans_helper.hpp
├── translation-------------------------# 符号表构建、类型检查、中间代码生成模块
|  ├── build_sym.cpp
|  ├── translation.cpp
|  ├── translation.hpp
|  └── type_check.cpp
└── type--------------------------------# 类型定义模块
|  ├── array_type.cpp
|  ├── base_type.cpp
|  ├── func_type.cpp
|  ├── type.cpp
|  └── type.hpp
├── location.hpp------------------------# 记录待编译源文件中的位置
├── options.cpp-------------------------# 处理运行选项，如上面提到的<level>和<machine>
├── options.hpp
├── compiler.cpp------------------------# 编译器主流程
├── compiler.hpp
├── config.hpp
├── define.hpp
├── error.cpp---------------------------# 对于编译错误的定义和处理
├── error.hpp
├── errorbuf.hpp
├── misc.cpp----------------------------# 杂乱的辅助函数
├── main.cpp
├── Makefile
```

## 说明

同学们在使用框架完成实验时遇到问题，欢迎随时 cue 助教！