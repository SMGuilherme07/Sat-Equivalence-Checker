#!/usr/bin/env bash
set -e

# Vai para a raiz do projeto.
cd -- "$(dirname -- "$0")/.."

mkdir -p resultados_massif/enorme

g++ -std=c++17 -O2 -g solver.cpp -o solver

valgrind --tool=massif \
    --time-unit=ms \
    --massif-out-file=resultados_massif/enorme/massif.out \
    ./solver enorme/f1.cnf enorme/f2_equivalente.cnf \
    > resultados_massif/enorme/saida.txt