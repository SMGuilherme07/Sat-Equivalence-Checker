#!/usr/bin/env bash
set -e

# Vai para a raiz do projeto.
cd -- "$(dirname -- "$0")/.."

mkdir -p resultados_heaptrack/enorme

heaptrack -o resultados_heaptrack/enorme/heaptrack_enorme \
    ./solver enorme/f1.cnf enorme/f2_equivalente.cnf \
    > resultados_heaptrack/enorme/saida.txt