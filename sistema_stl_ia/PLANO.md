# Plano de implementação do Cashly

Feito a partir do `DESCRICAO.md` (com as decisões da conversa já escritas nele)
e do `AGENTS.md`. Regras que valem para todas as etapas:

- C++11, somente biblioteca padrão;
- nenhuma classe usa `cin`/`cout` — quem fala com o usuário é o `main.cpp`;
- uma classe por par de arquivos (`include/X.hpp` + `src/X.cpp`);
- sem `new`/`delete`: os objetos ficam guardados direto nos containers;
- sem lambda.

Observação: a `DESCRICAO.md` escreve `Categorias` (plural) uma vez; vou usar
`Categoria` (singular), como no resto do texto.

---

## Arquivos que serão criados

| Arquivo | Conteúdo |
| --- | --- |
| `include/Gasto.hpp` + `src/Gasto.cpp` | classe `Gasto` |
| `include/Categoria.hpp` + `src/Categoria.cpp` | classe `Categoria` |
| `include/Carteira.hpp` + `src/Carteira.cpp` | classe `Carteira` |
| `src/main.cpp` | interface: menu, leitura e escrita |
| `testes/testar.sh` | script que compila e roda os testes |
| `testes/teste1.in` + `.out` ... | entradas de teste e saídas esperadas (criados nas etapas) |

O comando de compilação do `README.md` (`src/*.cpp -Iinclude`) já cobre tudo;
não precisa mudar.

## Como os testes funcionam

A pasta `testes/` guarda pares de arquivos: `testeN.in` (a entrada) e
`testeN.out` (a saída esperada). O script `testes/testar.sh` compila o
programa, roda cada `.in` e compara a saída com o `.out` de mesmo nome:

```bash
bash testes/testar.sh
```

Para criar um `.out`: escreva o `.in`, rode o programa uma vez com ele,
**confira na mão se a saída está correta** e só então salve como `.out`:

```bash
./sistema < testes/teste1.in > testes/teste1.out
```

---

## As classes

### `Gasto`

**O que guarda:** `id` (int), `data` (string DD/MM/AAAA), `descricao` (string),
`valor` (double), `categoria` (string).

Detalhe importante: a `categoria` guardada no gasto é **sempre o nome original
da categoria cadastrada**, não o que o usuário digitou ao registrar. Assim, se
o usuário digitar "ALIMENTACAO" no menu 03, o gasto fica com "Alimentação" e os
relatórios não se dividem em duas categorias diferentes.

**Construtores:**

- `Gasto()` — vazio;
- `Gasto(int id)` — só com o id, para o `find` da opção 09;
- `Gasto(int id, string data, string descricao, double valor, string categoria)`.

**Métodos:**

- getters de todos os campos;
- `static bool dataValida(const string& data)` — confere o formato DD/MM/AAAA
  (tamanho, barras e só dígitos) e se a data existe de verdade no calendário
  (dias do mês, inclusive ano bissexto: múltiplo de 4, exceto os múltiplos de
  100 que não são múltiplos de 400);
- `bool operator==(const Gasto& outro) const` — compara **só o id**. Motivo:
  o `std::find` da opção 09 procura o gasto pelo id;
- `bool operator<(const Gasto& outro) const` — compara o `valor`; em caso de
  empate, compara a `data`. Motivo: o `sort` da opção 06.

Por que funciona sem lambda: a data no formato DD/MM/AAAA sempre tem dígitos
alinhados (05/10, não 5/10 — a `dataValida` garante), então comparar a string
da data é comparar em ordem cronológica. Assim, `sort(v.begin(), v.end(),
greater<Gasto>())` ordena do maior valor para o menor e, no empate, do mais
recente para o mais antigo — exatamente a regra 54 da descrição.

### `Categoria`

**O que guarda:** `nome` (string, como o usuário digitou no cadastro) e
`limite` (double).

**Métodos:**

- getters e `setLimite`;
- `static string normalizar(const string& texto)` — devolve o texto todo em
  minúsculas e sem acentos. É o que faz "Alimentação", "ALIMENTACAO" e
  "alimentação" serem a mesma categoria (regra 61). Os acentos portugueses
  (á à â ã ç é ê í ó ô õ ú) ocupam 2 bytes no UTF-8; a função percorre os bytes
  e troca as sequências conhecidas por sua letra simples.

### `Carteira`

**O que guarda:**

- `vector<Gasto> gastos` — na ordem de registro (a opção 04 mostra nessa ordem);
- `map<string, Categoria> categorias` — a **chave** é o nome da categoria
  normalizado (minúsculas, sem acentos) e o **valor** é o objeto `Categoria`,
  que guarda o nome original e o limite. O map sozinho garante três coisas:
  não deixa chave repetida (regra do nome duplicado), a busca por nome é
  rápida (`find`) e o percurso sai em ordem alfabética (regra 62 e ordem da
  opção 02);
- `int proximoId` — começa em 1. Excluiu o gasto 3, o próximo registrado é o 4
  (ids não são reutilizados).

**Códigos de retorno.** Vários métodos podem dar errado por motivos
diferentes, e cada motivo tem um texto diferente na tela. Em vez de um
`bool`, esses métodos devolvem um `int` com o código do resultado, e a
interface traduz o código na mensagem. Os códigos ficam declarados no
`Carteira.hpp`:

```cpp
static const int OK = 0;
static const int CATEGORIA_INEXISTENTE = 1;
static const int VALOR_INVALIDO = 2;
static const int DATA_INVALIDA = 3;
static const int DESCRICAO_VAZIA = 4;
static const int NOME_VAZIO = 5;
static const int CATEGORIA_DUPLICADA = 6;
static const int LIMITE_INVALIDO = 7;
```

**Métodos** (nenhum deles usa `cin`/`cout`):

- `int cadastrarCategoria(const string& nome, double limite)` — recusa nome
  vazio, limite negativo e nome repetido: normaliza o nome e verifica se a
  chave já existe no map (`find`); se existir, devolve `CATEGORIA_DUPLICADA`.
  Se der certo, grava no map a chave normalizada apontando para uma
  `Categoria` que guarda o nome original e o limite;
- `const map<string, Categoria>& listarCategorias() const` — a interface
  percorre o map (já sai em ordem alfabética) e mostra nome original e limite
  de cada entrada;
- `int registrarGasto(const string& data, const string& descricao,
  const string& valorTexto, const string& categoria)` — valida tudo aqui
  dentro: categoria existe (normaliza e procura no map), descrição não vazia,
  data válida e valor. O valor vem como **texto** de propósito: o `Carteira`
  converte com `stringstream` e, se sobrar lixo na string (ex.: "45,90" ou
  "abc"), devolve `VALOR_INVALIDO`. Se tudo der certo, cria o gasto com
  `proximoId`, guarda no gasto o nome **original** da categoria (o nome que
  está no objeto `Categoria`, não o que o usuário digitou) e soma 1 no
  contador;
- `const vector<Gasto>& listarGastos() const`;
- `bool excluirGasto(int id)` — monta um `Gasto` só com o id e usa
  `std::find` (que compara com o `operator==`); achou, apaga com `erase`,
  não achou, devolve `false`;
- `map<string, double> totaisDoMes(int mes, int ano) const` — percorre o map
  de categorias e, para cada uma, soma os gastos do mês e do ano pedidos. O
  mapa de retorno usa o nome **original** como chave, então a interface
  recebe tudo em ordem alfabética, inclusive as categorias com total zero;
- `map<string, vector<Gasto>> gastosDoMesPorCategoria(int mes, int ano)
  const` — para a opção 06: primeiro varre os gastos do mês e insere os nomes
  das categorias com gasto num `set<string>` (não repete e já sai em ordem
  alfabética); depois, para cada nome do set, junta os gastos daquele mês e
  daquela categoria num `vector`, ordena com `sort` + `greater<Gasto>()` e
  grava no map de retorno. Categorias sem gasto no mês nem entram no set nem
  no mapa, então a regra "só aparecem categorias com gasto" sai de graça;
- `bool acimaDoLimite(const string& categoria, int mes, int ano) const` —
  procura a categoria no map (normalizando) e compara o limite com o total do
  mês; devolve `true` só quando o total for **maior** que o limite (igual não
  conta). Usado pelo aviso pós-registro e pela opção 07;
- `int alterarLimite(const string& nome, double novoLimite)` — normaliza o
  nome, procura no map e chama `setLimite` no objeto encontrado; recusa
  categoria inexistente e limite negativo.

Ainda dentro de `Carteira.cpp`, duas funções auxiliares que não são métodos
(existem só nesse arquivo): `extrairMes` e `extrairAno`, que pegam o mês e o
ano de uma data "DD/MM/AAAA" para os relatórios consultarem só o mês pedido.

---

## Onde entra cada coisa da STL (e por quê)

| Onde | O quê | Por quê |
| --- | --- | --- |
| `Carteira::categorias` | `map<string, Categoria>` | chave = nome normalizado; impede duplicado, busca por nome rápida e percurso em ordem alfabética (regras 47 e 62) |
| `Carteira::gastos` | `vector<Gasto>` | inserção no fim (registro), percurso em ordem de registro (opção 04) e `sort` para o relatório 06 |
| relatório da opção 06 | `set<string>` | reúne os nomes das categorias com gasto no mês: não repete e entrega em ordem alfabética |
| `Carteira::totaisDoMes` | `map<string, double>` | soma por categoria e já entrega as chaves em ordem alfabética (regra 62) |
| `Carteira::gastosDoMesPorCategoria` | `map<string, vector<Gasto>>` | agrupa os gastos do mês por categoria; só entram categorias com gasto |
| `Carteira::excluirGasto` | `find` + `operator==` do `Gasto` | acha o gasto pelo id, como no exemplo 11 da aula |
| relatório da opção 06 | `sort` + `greater<Gasto>()` + `operator<` do `Gasto` | ordem decrescente de valor com desempate pela data mais recente |

Isso atende todos os requisitos de STL do `ENUNCIADO.md`: um `map`, um `set`,
e `sort`/`find` aplicados a objetos de classe nossa com operador definido.

---

## A interface (`src/main.cpp`)

- Um laço principal que mostra o menu, lê a opção e chama a função da opção;
  opção inválida mostra o aviso e mostra o menu de novo;
- Uma função por opção do menu (ex.: `opcaoRegistrarGasto(Carteira&)`) para o
  `main` não virar um bloco gigante;
- **Leitura:** tudo com `getline` e conversão com `stringstream` — nunca mistura
  `cin >>` com `getline`, então não briga com o `\n` sobrando no buffer;
- **Valor:** o texto lido vira `double` com `stringstream`; se sobrar qualquer
  coisa na string ("45,90", "abc"), a própria `Carteira` rejeita;
- **Números na tela:** `fixed` + `setprecision(2)`, com "R$ " na frente;
- **Nomes de categoria nos relatórios:** saem como foram cadastrados (ex.:
  "Alimentação:"), não em MAIÚSCULAS — deixar acento em maiúscula com
  `toupper` byte a byte bagunça o UTF-8.

**Mensagens de erro** (os textos exatos, para os testes):

| Situação | Mensagem |
| --- | --- |
| opção inválida no menu | `Opção inválida!` |
| categoria já cadastrada | `Categoria já cadastrada!` |
| nome de categoria vazio | `Nome de categoria vazio!` |
| limite negativo (01 ou 08) | `Limite inválido!` |
| categoria inexistente (03 ou 08) | `Categoria Inexistente!` |
| valor zero, negativo ou não numérico | `Valor inválido!` |
| data com formato errado ou impossível | `Data inválida!` |
| descrição vazia | `Descrição vazia!` |
| id não existe no 09 | `Nenhum gasto encontrado com esse id!` |
| sem categorias | `Nenhuma categoria registrada` |
| sem gastos | `Nenhum gasto registrado` |
| relatório de mês sem gastos | `Nenhum gasto registrado no período selecionado` |
| categoria acima do limite | `ATENÇÃO: <nome> estourou o limite! Gasto: R$ <total> - Limite: R$ <limite> - Excedido: R$ <total-limite>` |

---

## Etapas

Cada etapa termina compilando (`g++ -std=c++11 -Wall -Wextra src/*.cpp
-Iinclude -o sistema`) e com algo que dá para testar na mão. Uma etapa = um
commit.

### Etapa 1 — Classes `Gasto` e `Categoria`

- Cria `Gasto.hpp`/`.cpp` e `Categoria.hpp`/`.cpp` com construtores, getters e
  operadores;
- Implementa `Gasto::dataValida` (formato + calendário + bissexto) e
  `Categoria::normalizar`;
- `main.cpp` de teste provisório: cria objetos e imprime se datas como
  "29/02/2024", "31/02/2026", "12/10/2026" e "abc" são válidas, e o que
  `normalizar("Alimentação")` devolve.

**Como testar:** compilar e rodar; conferir as respostas na tela.

### Etapa 2 — `Carteira`: categorias e o esqueleto do menu

- Cria `Carteira.hpp`/`.cpp` com o `map<string, Categoria>` e o `proximoId`;
- Implementa `cadastrarCategoria` (com a checagem de duplicado pelo `find` no
  map) e `listarCategorias`;
- `main.cpp` vira o menu de verdade: opções 01, 02 e 00, mais o aviso de
  opção inválida.

**Como testar:** cadastrar "Lazer" 150, depois "alimentação" 500, depois
"ALIMENTACAO" de novo (deve dar erro de duplicado), listar (deve sair em
ordem alfabética: alimentação, lazer), sair.

### Etapa 3 — Registrar gasto (opção 03)

- Implementa `registrarGasto` com todas as validações (categoria com
  normalização, descrição, data, valor como texto) e o código de retorno;
- A interface mostra o texto do código e, no sucesso, "Gasto registrado com o
  id N!".

**Como testar:** registrar certo; registrar com categoria inexistente; com
valor "45,90"; com data "31/02/2026"; com descrição vazia.

### Etapa 4 — Listar gastos (04) e excluir gasto (09)

- Implementa `listarGastos` (mostra id, data, descrição, valor e categoria, na
  ordem de registro) e `excluirGasto` com `find` + `operator==`;
- Interface: opções 04 e 09.

**Como testar:** listar; excluir um id que existe; excluir um id que não
existe; listar de novo.

### Etapa 5 — Total por categoria (opção 05) + primeiro teste

- Implementa `totaisDoMes` com o `map<string, double>`;
- Interface: opção 05 com cabeçalho "Resumo de Gastos - <Mês>/<Ano>", alinhado
  como no exemplo do `DESCRICAO.md`;
- Cria `testes/teste1.in` com o IN do `DESCRICAO.md` (cadastra as três
  categorias, registra almoço e combustível, consulta 10/2026, sai) e o
  `testes/teste1.out` com a saída conferida.

**Como testar:** `bash testes/testar.sh` e conferir que o teste1 passou; a
saída deve ser igual à do `DESCRICAO.md` (Alimentação 45.90, Lazer 0.00,
Transporte 200.00, em ordem alfabética).

### Etapa 6 — Maiores gastos do mês (opção 06) + teste

- Implementa `gastosDoMesPorCategoria` com o `map<string, vector<Gasto>>` e o
  `sort` + `greater<Gasto>()`;
- Interface: opção 06, com uma seção por categoria e os gastos do maior para o
  menor;
- Cria `testes/teste2.in` com gastos no mesmo valor (para testar o
  desempate pela data) e um mês com gasto só em uma categoria; confere a
  saída e salva o `testes/teste2.out`.

**Como testar:** `bash testes/testar.sh` e conferir que teste1 e teste2
passam: ordem por categoria, ordem decrescente dentro da categoria e o
desempate.

### Etapa 7 — Verificar limites (07), alterar limite (08) e aviso (03)

- Implementa `acimaDoLimite` e `alterarLimite`;
- Interface: opção 07 mostra, por categoria, limite, utilizado e disponível, e
  o aviso `ATENÇÃO: ... estourou o limite!` para as que passaram; opção 08;
- O menu 03 passa a mostrar o mesmo aviso logo após registrar um gasto que
  estourou o limite;
- Cria `testes/teste3.in` cobrindo 07, 08 (inclusive alterar para um
  limite menor que o total e ver o aviso) e 09 com id inexistente; confere a
  saída e salva o `testes/teste3.out`.

**Como testar:** `bash testes/testar.sh` — os três testes devem passar;
compilar com `-Wall -Wextra` e ficar sem avisos.

---

## Coisas que eu escolhi e você pode mudar

- Os textos das mensagens de erro (a tabela acima);
- A opção 02 lista as categorias em ordem alfabética (é a ordem natural do map;
  a descrição não dizia nada sobre essa listagem);
- Nos relatórios, o nome da categoria sai como cadastrado, não em maiúsculas;
- Ao rodar com `< arquivo`, os prompts ("Digite a data: ") aparecem misturados
  com o relatório na saída — é o comportamento normal de quem escreve na tela;
- A classe se chama `Categoria` (singular), como no resto da descrição.

Se algum desses pontos incomodar, diga antes de a gente começar a etapa 1.
