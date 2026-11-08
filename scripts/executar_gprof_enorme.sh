#!/usr/bin/env bash
set -e

cd -- "$(dirname -- "$0")/.."

mkdir -p resultados_gprof/enorme

g++ -std=c++17 -O2 -g -pg solver.cpp -o solver_gprof

for i in $(seq 1 51); do
    ./solver_gprof enorme/f1.cnf enorme/f2_equivalente.cnf \
        > resultados_gprof/enorme/saida.txt

    mv gmon.out "resultados_gprof/enorme/gmon_$i.out"
done