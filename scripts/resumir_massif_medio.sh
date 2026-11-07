#!/usr/bin/env bash

# Execute a partir da raiz do repositorio.
# Extrai o maior heap total registrado em cada execucao.

(
    echo "execucao,maximo_heap_total_bytes"

    for i in {1..51}; do
        awk -F= -v execucao="$i" '
            /^mem_heap_B=/       { heap = $2 }
            /^mem_heap_extra_B=/ {
                total = heap + $2
                if (total > maximo)
                    maximo = total
            }
            END {
                printf "%d,%.0f\n", execucao, maximo
            }
        ' "resultados_massif/medio/massif_$i.out"
    done
) > resultados_massif/medio/resumo_memoria.csv
