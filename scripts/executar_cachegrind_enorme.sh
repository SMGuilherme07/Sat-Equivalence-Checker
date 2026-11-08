#!/usr/bin/env bash
set -e

# Vai para a raiz do projeto.
cd -- "$(dirname -- "$0")/.."

mkdir -p resultados_cachegrind/enorme

valgrind --tool=cachegrind \
    --cache-sim=yes \
    --branch-sim=yes \
    --cachegrind-out-file=resultados_cachegrind/enorme/cachegrind.out \
    --log-file=resultados_cachegrind/enorme/valgrind.log \
    ./solver enorme/f1.cnf enorme/f2_equivalente.cnf \
    > resultados_cachegrind/enorme/saida.txt