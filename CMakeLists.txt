cmake_minimum_required(VERSION 3.10)
project(my-lisp)
set(CMAKE_C_STANDARD 99)
add_executable(mlsp src/arena.c src/eval.c src/helper.c src/lexer.c
               src/parser.c src/main.c)
