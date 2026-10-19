#include "Carteira.hpp"

#include <algorithm>
#include <functional>
#include <set>
#include <sstream>

// Auxiliares que existem só neste arquivo: pegam o mês e o ano de uma
// data "DD/MM/AAAA" (a data já passou pela dataValida no registro).
static int extrairMes(const string& data) {
    return (data[3] - '0') * 10 + (data[4] - '0');
}

static int extrairAno(const string& data) {
    return (data[6] - '0') * 1000 + (data[7] - '0') * 100
         + (data[8] - '0') * 10 + (data[9] - '0');
}

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

int Carteira::registrarGasto(const string& data, const string& descricao,
                             const string& valorTexto, const string& categoria) {
    string chave = Categoria::normalizar(categoria);
    map<string, Categoria>::iterator it = categorias.find(chave);
    if (it == categorias.end()) {
        return CATEGORIA_INEXISTENTE;
    }
    if (descricao.empty()) {
        return DESCRICAO_VAZIA;
    }
    if (!Gasto::dataValida(data)) {
        return DATA_INVALIDA;
    }

    istringstream stream(valorTexto);
    double valor = 0.0;
    stream >> valor;
    if (stream.fail() || valor <= 0.0) {
        return VALOR_INVALIDO;
    }
    stream >> ws;
    if (!stream.eof()) {
        return VALOR_INVALIDO;
    }

    Gasto gasto(proximoId, data, descricao, valor, it->second.getNome());
    gastos.push_back(gasto);
    proximoId++;
    return OK;
}

const vector<Gasto>& Carteira::listarGastos() const {
    return gastos;
}

bool Carteira::excluirGasto(int id) {
    Gasto procurado(id);
    vector<Gasto>::iterator it = find(gastos.begin(), gastos.end(), procurado);
    if (it == gastos.end()) {
        return false;
    }
    gastos.erase(it);
    return true;
}

int Carteira::ultimoId() const {
    return proximoId - 1;
}

map<string, double> Carteira::totaisDoMes(int mes, int ano) const {
    map<string, double> totais;
    map<string, Categoria>::const_iterator cat;
    for (cat = categorias.begin(); cat != categorias.end(); ++cat) {
        double total = 0.0;
        for (vector<Gasto>::size_type i = 0; i < gastos.size(); ++i) {
            const Gasto& gasto = gastos[i];
            if (extrairMes(gasto.getData()) != mes) {
                continue;
            }
            if (extrairAno(gasto.getData()) != ano) {
                continue;
            }
            if (Categoria::normalizar(gasto.getCategoria()) == cat->first) {
                total += gasto.getValor();
            }
        }
        totais[cat->second.getNome()] = total;
    }
    return totais;
}

map<string, vector<Gasto> > Carteira::gastosDoMesPorCategoria(int mes,
                                                              int ano) const {
    // Primeiro: quais categorias tiveram gasto no mês. O set não deixa
    // repetir e já entrega os nomes em ordem alfabética.
    set<string> comGasto;
    for (vector<Gasto>::size_type i = 0; i < gastos.size(); ++i) {
        const Gasto& gasto = gastos[i];
        if (extrairMes(gasto.getData()) == mes
                && extrairAno(gasto.getData()) == ano) {
            comGasto.insert(gasto.getCategoria());
        }
    }

    // Depois: junta os gastos de cada categoria do set e ordena do maior
    // valor para o menor (no empate, a data mais recente vem primeiro).
    map<string, vector<Gasto> > porCategoria;
    set<string>::const_iterator nome;
    for (nome = comGasto.begin(); nome != comGasto.end(); ++nome) {
        vector<Gasto> doMes;
        for (vector<Gasto>::size_type i = 0; i < gastos.size(); ++i) {
            const Gasto& gasto = gastos[i];
            if (extrairMes(gasto.getData()) == mes
                    && extrairAno(gasto.getData()) == ano
                    && gasto.getCategoria() == *nome) {
                doMes.push_back(gasto);
            }
        }
        sort(doMes.begin(), doMes.end(), greater<Gasto>());
        porCategoria[*nome] = doMes;
    }
    return porCategoria;
}

bool Carteira::acimaDoLimite(const string& categoria, int mes,
                             int ano) const {
    map<string, Categoria>::const_iterator it =
        categorias.find(Categoria::normalizar(categoria));
    if (it == categorias.end()) {
        return false;
    }

    double total = 0.0;
    for (vector<Gasto>::size_type i = 0; i < gastos.size(); ++i) {
        const Gasto& gasto = gastos[i];
        if (extrairMes(gasto.getData()) == mes
                && extrairAno(gasto.getData()) == ano
                && Categoria::normalizar(gasto.getCategoria()) == it->first) {
            total += gasto.getValor();
        }
    }

    // Só é "acima" quando passa do limite; igual ao limite não conta.
    return total > it->second.getLimite();
}

int Carteira::alterarLimite(const string& nome, double novoLimite) {
    if (novoLimite < 0) {
        return LIMITE_INVALIDO;
    }
    map<string, Categoria>::iterator it =
        categorias.find(Categoria::normalizar(nome));
    if (it == categorias.end()) {
        return CATEGORIA_INEXISTENTE;
    }
    it->second.setLimite(novoLimite);
    return OK;
}
