# Descrição do sistema

## Nome e ideia

O Cashly é um sistema de controle de gasto que auxilia o usuário a organizar suas despesas pessoais, registrando quanto foi gasto, quando isso ocorreu e em  qual categoria cada gasto se enquadra. O sistema permite acompanhar o total gasto por categoria, identificar os maiores gastos de cada mês e verificar as categorias que ultrapassaram os limites de gastos que foram estabelecidos pelo usuário.

## O que o sistema guarda

O sistema possui três classes principais: Gasto, Categorias e Carteira.

`Gasto` representa uma despesa registrada pelo usuário. Guarda as informações necessárias para identificar e consultar cada gasto:

- id (`int`) - identificador único do gasto;
- data (`string`) - data em que o gasto foi realizado, preferivelmente no formato DD/MM/AAAA;
- descricao (`string`) - descrição do gasto, por exemplo almoço, supermercado, farmácia e etc;
- valor (`double`) - valor que foi gasto;
- categoria (`string`) - nome da categoria à qual o gasto pertence.

`Categoria` representa uma categoria utilizada para organizar os gastos. Cada categoria deve possuir um limite mensal de gastos definido pelo usuário.

- nome (`string`) - nome da categoria;
- limite (`double`) - valor máximo de gastos permitidos por mês para aquela categoria.

`Carteira` representa o "gerenciador" do sistema. É a parte responsável por armazenar os gastos e categorias cadastradas, além de realizar os cálculos e gerar os relatórios.

- gastos (`vector<Gasto>`) - coleção de gastos registrados pelo usuário;
- categorias (`vector<Categorias>`) - coleção de categorias cadastradas;

## O que dá para fazer

O cashly terá um menu com as seguintes opções:

01. cadastrar categoria - solicita o nome da categoria e o limite mensal de gasto;
02. listar categorias - mostra as categorias cadastradas e seus respectivos limites mensais;
03. registrar gasto - solicita a data, descrição, valor e categoria. O id é associado automaticamente a cada resgistro;
04. listar gastos - exibe os gastos registrados;
05. consultar total por categoria - solicita o mês e o ano e mostra quanto foi gasto em cada categoria no período definido;
06. listar maiores gastos do mês - solicita mês e ano e exibe os maiores gastos por categorias em ordem decrescente;
07. verificar limites - mostra as categorias e os respectivos limites com os tanto que tem disponível e quanto já foi utilizado;
08. alterar limite - permite modificar o limite mensal de uma categoria existente;
09. excluir gasto - permite remover um gasto registrados a partit do seu identificador;
00. sair - encerra o sistema.


## Regras

- o nome de uma categoria não pode ser vazio nem duplicado;
- o limite mensal de uma categoria deve ser maior ou igual a zero;
- todo gasto deve possuir todos os parâmetros (data, descrição, valor e categoria);
- não será permitido registrar gastos em categorias inexistentes;
- cada gasto terá um identificador único, gerado automaticamente pelo sistema;
- os valores serão exibidos com duas casas decimais;
- os totais por categoria devem considerar apenas os gastos do mês e do ano selecionado;
- os maiores gastos do mês serão apresentados em ordem decrescente de valor. Em caso de empate, serão ordenados pela data, do mais recente para o mais antigo;
- uma categoria será considerada acima do limite somente quando o total mensal de gastos for maior que o limite definido. Se o total for igual ao limite, ela não será considerada ultrapassada.
-  ao alterar um limite, os gastos já registrados não serão modificados; apenas as consultas futuras utilizarão o novo limite.
- os gastos de meses diferentes devem ser contabilizados separadamente, mesmo quando pertecem à mesma categoria. 

## Quando algo dá errado

- quando o usuário tenta registrar um gasto ou alterar um limite usando uma categoria não cadastrada, o sistema mostrará uma mensagem de erro ("Categoria Inexistente!");
- se o usuário tentar excluir um gasto cujo identificador não existe, o sistema informará que nenhum gasto foi encontrado;
- se não houver gastos ou categorias cadastradas, o sistema exibirá "Nenhum gasto registrado" ou "Nenhuma categoria registrada";
- se o valor de um gasto for zero, negativo ou não representar um número válido, o registro será recusado e o sistema exibirá uma mensagem como "Valor inválido";
- se a data não existir no calendário, o sistema não permitirá o cadastro;
- o sistema mostrará um aviso com o nome da categoria, o valor gasto, o limite mensal e quanto foi excedido;
- se não houver gastos no mês e ano selecionados, os relatórios mostrarão uma mensagem informando que não existem gastos registrados para o período selecionado;
- se o usuário digitar uma opção inválida no menu, o sistema informará que a opção é inválida e solicitará uma nova escolha.


## Classes que eu imagino

- `Gasto` representa uma despesa registrada pelo usuário. Guarda as informações necessárias para identificar e consultar cada gasto.
- `Categoria` representa uma categoria utilizada para organizar os gastos. Cada categoria deve possuir um limite mensal de gastos definido pelo usuário.
- `Carteira` representa o "gerenciador" do sistema. É a parte responsável por armazenar os gastos e categorias cadastradas, além de realizar os cálculos e gerar os relatórios.

## Exemplo de uso

IN
```
CADASTRAR_CATEGORIA Alimentação 500.00
CADASTRAR_CATEGORIA Transporte 200.00
CADASTRAR_CATEGORIA Lazer 150.00
REGISTRAR_GASTO 12/10/2026 almoço 45.90 Alimentação
REGISTRAR_GASTO 14/09/2026 supermercado 127.85 Alimentação
REGISTRAR_GASTO 12/10/2026 combustível 200.00 Transporte
CONSULTAR_TOTAL_POR_CATEGORIA 10 2026
FIM
```

OUT

```
Resumo de Gastos - Outubro/2026

Transporte ----------- R$ 200.00 (Limite: R$ 200.00)
Alimentação ---------- R$ 45.90  (Limite: R$ 500.00)
Lazer ---------------- R$ 0.00   (Limite: R$ 150.00)
```
