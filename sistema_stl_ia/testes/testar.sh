#!/usr/bin/env bash
# Uso: bash testes/testar.sh
#
# Compila o programa e roda cada arquivo .in da pasta testes/, comparando a
# saida com o .out de mesmo nome. Rode a partir da raiz do repositorio.

set -u

pasta="testes"
if [ ! -d "$pasta" ]; then
  echo "Pasta $pasta nao existe."
  exit 1
fi

# Compila tudo que estiver em src/, buscando os .hpp em include/.
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema || exit 1

total=0
falhas=0
for entrada in "$pasta"/*.in; do
  # Se nao existir nenhum .in ainda, sai do laço.
  [ -e "$entrada" ] || break

  esperado="${entrada%.in}.out"
  nome="$(basename "$entrada" .in)"
  total=$((total + 1))

  if [ ! -f "$esperado" ]; then
    echo "FALHA $nome (falta o arquivo $esperado)"
    falhas=$((falhas + 1))
    continue
  fi

  saida="$(./sistema < "$entrada")"
  if [ "$saida" == "$(cat "$esperado")" ]; then
    echo "OK    $nome"
  else
    echo "FALHA $nome"
    echo "----- diferenca (esperado com -, seu programa com +) -----"
    diff -u "$esperado" <(echo "$saida") | tail -n +3 | head -30
    echo "-----------------------------------------------------------"
    falhas=$((falhas + 1))
  fi
done

echo
echo "$((total - falhas)) de $total testes passaram."
if [ "$falhas" -ne 0 ]; then
  exit 1
fi
