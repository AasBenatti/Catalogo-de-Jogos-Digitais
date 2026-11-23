cmake_minimum_required(VERSION 3.10)
project(catalogo C)

set(CMAKE_C_STANDARD 99)

add_executable(catalogo main.c jogo.c jogo.h)

# Caminho absoluto: o programa sempre usa o dados.txt da pasta do projeto,
# nao importa de onde o CLion execute
target_compile_definitions(catalogo PRIVATE "ARQUIVO=\"${CMAKE_SOURCE_DIR}/dados.txt\"")
