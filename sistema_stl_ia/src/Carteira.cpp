#include "Carteira.hpp"

Carteira::Carteira() : proximoId(1) {
}

int Carteira::cadastrarCategoria(const string& nome, double limite) {
    if (nome.empty()) {
        return NOME_VAZIO;
    }
    if (limite < 0) {
        return LIMITE_INVALIDO;
    }
    string chave = Categoria::normalizar(nome);
    map<string, Categoria>::iterator it = categorias.find(chave);
    if (it != categorias.end()) {
        return CATEGORIA_DUPLICADA;
    }
    categorias[chave] = Categoria(nome, limite);
    return OK;
}

const map<string, Categoria>& Carteira::listarCategorias() const {
    return categorias;
}
