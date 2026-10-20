# Cashly - controle de gastos com STL

Atividade da Unidade 2 de Linguagem de Programação I. Leia `ENUNCIADO.md`
antes de começar.

Uma frase dizendo o que o sistema faz. A descrição completa está em
`DESCRICAO.md`.

## Arquivos

```text
.
├── AGENTS.md       regras que a IA lê antes de cada pedido
├── DESCRICAO.md    a descrição do sistema, escrita por você
├── ENUNCIADO.md    a atividade
├── PLANO.md        o plano feito com a IA (criado na etapa 3)
├── README.md       este arquivo
├── testes/         arquivos de entrada para testar
|   ├── testar.sh        compila e roda os testes de uma pasta
|   ├── teste1.in        testes das etapas 01 a 05 
|   ├── teste1.out       testes das etapas 01 a 05
|   ├── teste2.in        testes da etapa 06
|   ├── teste2.out       testes da etapa 06
|   ├── teste3.in        testes da etapa 07 
|   └── teste3.out       testes da etapa 07
├── include/        um .hpp por classe
|   ├── Carteira.hpp     
|   ├── Categoria.hpp    
|   └── Gasto.hpp        
└── src/            um .cpp por classe, mais a interface
    ├── Carteira.hpp        
    ├── Categoria.hpp    
    ├── Gasto.hpp       
    └── main.cpp       
```

## Compilar e executar

```bash
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema
./sistema
```

Para rodar os testes (compara a saída do programa com a saída esperada de
cada `.in` da pasta `testes/`):

```bash
bash testes/testar.sh
```

## Onde uso a STL

Escreva esta seção sem IA. Para cada container ou algoritmo da STL que o
sistema usa: em que classe ele está, o que guarda, e por que você escolheu
ele e não outro.

```text
|                Onde                |               O quê               |                   Por quê                      |
|------------------------------------|-----------------------------------|------------------------------------------------|
| include/Carteira.hpp               | map<string, Categorias>           | tem alguns comandos que eu preciso procurar    |
|                                    |                                   | pelo nome e o `map` faz isso sozinho, não deixa|
|                                    |                                   | chave repetida e ainda entrega a lista em ordem|
|                                    |                                   | alfabética. Se eu usasse um `vector`, a busca  |
|                                    |                                   | seria manual e não impediria duplicata.        |
|------------------------------------|-----------------------------------|------------------------------------------------|
| include/Carteira.hpp               | vector<Gasto>                     | as opções 04, 05, 06 e 07 percorrem a lista in-|
|                                    |                                   | teira e a opção 04 mostra na ordem de registro |
|                                    |                                   | o vector percorre por índice, de forma simples.|
|                                    |                                   | Se usasse um list não teria acesso por índice. |
|------------------------------------|-----------------------------------|------------------------------------------------|
| src/Carteira.cpp                   | set<string>                       | o set não repete e já sai em ordem alfabética: |
|                                    |                                   | se Alimentação teve 3 gastos no mês, o nome    |
|                                    |                                   | entra uma vez só. Um vector guardaria "Alimen- |
|                                    |                                   |tação" três vezes e eu teria que tirar a repeti-| 
|                                    |                                   | na mão.                                        |
|------------------------------------|-----------------------------------|------------------------------------------------|
| src/Carteira.cpp                   | map<string, double>               | mesma vantagem do map de categorias: busca pelo|
|                                    |                                   | nome sem repetir, em ordem alfabética (é o que |
|                                    |                                   | faz a opção 05 sair ordenada sem ordenar nada).|
|------------------------------------|-----------------------------------|------------------------------------------------| 
| src/Carteira.cpp                   | map<string, vector<Gasto>>        |o map organiza uma seção por categoria, em ordem|
|                                    |                                   |alfabética; o vector de cada categoria guarda os|
|                                    |                                   |gastos para o sort ordenar do maior p/ o menor. |
|------------------------------------|-----------------------------------|------------------------------------------------|
| src/Carteira.cpp                   | sort + greater<Gasto>()           | é o algoritmo padrão para ordenar um vector;   |
|                                    |                                   | greater inverte a ordem usando o operator> do  |
|                                    |                                   | Gasto (o desempate pela data fica no operator</|
|                                    |                                   | operator>). partial_sort só seria útil se      |
|                                    |                                   | quiséssemos só os 3 maiores — aqui mostramos   |
|                                    |                                   | todos.                                         |
|------------------------------------|-----------------------------------|------------------------------------------------|
| src/Carteira.cpp                   | find (algoritmo de <algorithm>)   | para comparar os ids um a um.                  |
|------------------------------------|-----------------------------------|------------------------------------------------|

```

## Como usei a IA

Escreva com as suas palavras:

*- Modelo usado:* Big Pickle

*- Um pedido que deu certo, e por que você acha que deu certo:* Solicitei que a IA criasse a etapa 7, referente as opções 07, 08 e o aviso de estouro. Deu certo porque além de ter uma descrição clara em PLANO.md e DESCRICAO.md, informei detalhes adicionais como: em Disponível, o saldo fica negativo quando o limite é ultrapassado; e, o aviso de estouro aparece embaixo da categoria. Ou seja, dei informações claras e exemplos de saída para que a IA não precisasse adivinhar.

*- Um pedido que deu errado, o que veio errado e como você mudou o pedido:* Quando a IA estava implementando a etapa referente ao item 06 do menu, o relatório dos maiores gastos por categoria, ela estava passando informações de id em toda linha e a categoria repetida no fim, mesmo já aparecendo no cabeçalho da seção, da mesma forma que aparece ao escolher o item 04. Dessa maneira:  
```
Alimentação:
2 | 12/10/2026 | lanche | R$ 45.90 | Alimentação.  
```
Então solicitei detalhadamente o que queria mudar e passei um exemplo do antes e da saída esperada. Dessa maneira:
```
Precisamos fazer uma alteração referente a saída da opção 06. Já que será sepada por categoria, não é interessante que a categoria apareça duas vezes, basta aparecer apenas no inicio da listagem e também não é necessário que apareça o id do registro, por exemplo: 

saída atual:
Alimentação:
2 | 12/10/2026 | lanche | R$ 45.90 | Alimentação
Lazer:
7 | 05/10/2026 | parque aquático | R$ 80.00 | Lazer
Saúde:
4 | 16/10/2026 | suplemento | R$ 200.00 | Saúde
3 | 15/10/2026 | farmácia | R$ 150.00 | Saúde
Transporte:
6 | 03/10/2026 | passagens | R$ 75.80 | Transporte
5 | 10/10/2026 | gasolina | R$ 50.00 | Transporte

saída esperada: 
Alimentação:
12/10/2026 | lanche | R$ 45.90 

Lazer:
05/10/2026 | parque aquático | R$ 80.00 

Saúde:
16/10/2026 | suplemento | R$ 200.00 
15/10/2026 | farmácia | R$ 150.00 

Transporte:
03/10/2026 | passagens | R$ 75.80 
10/10/2026 | gasolina | R$ 50.00 
```

*- Uma coisa que a IA sugeriu e você recusou, e por quê:* A IA tinha comprado minha ideia inicial de colocar um vector<Categorias>, mas depois de ler o plano que ela sugeriu, constatei que seria melhor usar um map<string, Categorias> pois evitaria alguns gargalos que o sistema poderia ter, como por exemplo, problemas com duplicidade, ordenação e afins. Por isso, solicitei a troca. Além disso, durante a implementação das etapas, constatei que ao realizar os testes com as entradas do usuário, o sistema, ao selecionar a opção 03 do menu, correspondente ao registro de gasto, permitia que o usuário inserisse todos os valores de todos os parâmetros ao invés de informar que não era possivel realizar o registro pq não havia nenhuma categoria cadastrada. A ideia seria alertar sobre o erro assim que ele enviasse o valor e não esperar para avisar o erro no final de toda a inserção.
