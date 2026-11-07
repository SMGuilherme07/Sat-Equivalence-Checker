#!/usr/bin/env bash

# Execute a partir da raiz do repositorio.
# Compilacao utilizada:
# g++ -std=c++17 -O2 -g solver.cpp -o solver

mkdir -p resultados_massif/medio

for i in {1..51}; do
    echo "Executando Massif: teste $i de 51"

    valgrind --tool=massif \
        --massif-out-file="resultados_massif/medio/massif_$i.out" \
        ./solver medio/f1.cnf medio/f2_equivalente.cnf \
        > "resultados_massif/medio/saida_$i.txt" \
        2> "resultados_massif/medio/log_$i.txt"

    if [ $? -ne 0 ]; then
        echo "ERRO no teste $i. A coleta foi interrompida."
        break
    fi

    ms_print "resultados_massif/medio/massif_$i.out" \
        > "resultados_massif/medio/relatorio_$i.txt"
done
