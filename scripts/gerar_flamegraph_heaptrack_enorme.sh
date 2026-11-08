#!/usr/bin/env bash
set -e

cd -- "$(dirname -- "$0")/.."

PASTA="resultados_heaptrack/enorme"

heaptrack_print "$PASTA/heaptrack_enorme.gz" \
    -F "$PASTA/pilhas_alocacoes.txt" \
    > "$PASTA/relatorio_flamegraph.txt"

perl ferramentas/FlameGraph/flamegraph.pl \
    --title "Heaptrack - alocacoes - entrada enorme" \
    --colors mem \
    --countname allocations \
    "$PASTA/pilhas_alocacoes.txt" \
    > "$PASTA/flamegraph_alocacoes.svg"

echo "Gerado: $PASTA/flamegraph_alocacoes.svg"