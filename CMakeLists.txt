cmake_minimum_required(VERSION 3.24)
project(compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(compiler
        main.cpp
        src/vm/vm.h
        src/frontend/parser/parser.h
        src/util.h
        src/frontend/lexer/lexer.h
        src/frontend/common/token.h
        src/frontend/parser/parser.h
        src/frontend/parser/parser.h
        src/frontend/lexer/fsm/regexParser.cpp
        src/frontend/lexer/fsm/regexParser.h
        src/frontend/lexer/fsm/DFA.cpp
        src/frontend/lexer/fsm/DFA.h
        src/frontend/lexer/fsm/NFABuilder.cpp
        src/frontend/lexer/fsm/NFABuilder.h
        src/frontend/lexer/fsm/NFA.cpp
        src/frontend/lexer/fsm/NFA.h
        src/frontend/lexer/fsm/FSM.h
        src/frontend/language/language.cpp
        src/frontend/language/language.h
        src/common/syntax_tree.h
        src/common/algorithms.h
        src/frontend/lexer/fsm/FSM.cpp
        src/frontend/lexer/lexer.cpp
        src/frontend/lexer/lexer.cpp
        src/frontend/common/token.cpp
        src/compile_lexer.cpp
        src/compile_lexer.cpp)
