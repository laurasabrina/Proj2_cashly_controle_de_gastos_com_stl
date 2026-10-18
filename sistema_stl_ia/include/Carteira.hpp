#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include <map>
#include <string>
#include <vector>

#include "Categoria.hpp"
#include "Gasto.hpp"

using namespace std;

class Carteira {
public:
    static const int OK = 0;
    static const int CATEGORIA_INEXISTENTE = 1;
    static const int VALOR_INVALIDO = 2;
    static const int DATA_INVALIDA = 3;
    static const int DESCRICAO_VAZIA = 4;
    static const int NOME_VAZIO = 5;
    static const int CATEGORIA_DUPLICADA = 6;
    static const int LIMITE_INVALIDO = 7;

    Carteira();

    int cadastrarCategoria(const string& nome, double limite);
    const map<string, Categoria>& listarCategorias() const;
    int registrarGasto(const string& data, const string& descricao,
                       const string& valorTexto, const string& categoria);
    int ultimoId() const;

private:
    vector<Gasto> gastos;
    map<string, Categoria> categorias;
    int proximoId;
};

#endif
