#!/usr/bin/env bash
set -e

# Vai para a raiz do projeto.
cd -- "$(dirname -- "$0")/.."

PASTA="resultados_gprof/enorme/figuras"

mkdir -p "$PASTA"

# Converte o relatorio corrigido em grafo.
"$HOME/.venvs/profiling/bin/gprof2dot" -f prof -s \
    resultados_gprof/enorme/relatorio_gprof_corrigido.txt \
    -o "$PASTA/chamadas.dot"

# Gera as imagens.
dot -Tpng "$PASTA/chamadas.dot" -o "$PASTA/chamadas.png"
dot -Tsvg "$PASTA/chamadas.dot" -o "$PASTA/chamadas.svg"

echo "Grafo gerado em: $PASTA"