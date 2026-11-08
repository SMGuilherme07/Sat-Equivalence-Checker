#!/usr/bin/env bash
set -e

# Vai para a raiz do projeto.
cd -- "$(dirname -- "$0")/.."

mkdir -p resultados_callgrind/enorme

valgrind --tool=callgrind \
    --callgrind-out-file=resultados_callgrind/enorme/callgrind.out \
    --log-file=resultados_callgrind/enorme/valgrind.log \
    ./solver enorme/f1.cnf enorme/f2_equivalente.cnf \
    > resultados_callgrind/enorme/saida.txt