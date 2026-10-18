#include "Gasto.hpp"

using namespace std;

Gasto::Gasto() : id(0), valor(0.0) {
}

Gasto::Gasto(int id) : id(id), valor(0.0) {
}

Gasto::Gasto(int id, string data, string descricao, double valor,
             string categoria)
    : id(id), data(data), descricao(descricao), valor(valor),
      categoria(categoria) {
}

int Gasto::getId() const {
    return id;
}

string Gasto::getData() const {
    return data;
}

string Gasto::getDescricao() const {
    return descricao;
}

double Gasto::getValor() const {
    return valor;
}

string Gasto::getCategoria() const {
    return categoria;
}

static bool bissexto(int ano) {
    if (ano % 400 == 0) {
        return true;
    }
    if (ano % 100 == 0) {
        return false;
    }
    return ano % 4 == 0;
}

bool Gasto::dataValida(const string& data) {
    if (data.size() != 10) {
        return false;
    }
    if (data[2] != '/' || data[5] != '/') {
        return false;
    }
    for (string::size_type i = 0; i < data.size(); ++i) {
        if (i == 2 || i == 5) {
            continue;
        }
        if (data[i] < '0' || data[i] > '9') {
            return false;
        }
    }

    int dia = (data[0] - '0') * 10 + (data[1] - '0');
    int mes = (data[3] - '0') * 10 + (data[4] - '0');
    int ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100
            + (data[8] - '0') * 10 + (data[9] - '0');

    if (mes < 1 || mes > 12) {
        return false;
    }
    if (dia < 1) {
        return false;
    }

    static const int diasPorMes[] = {31, 28, 31, 30, 31, 30,
                                     31, 31, 30, 31, 30, 31};
    int dias = diasPorMes[mes - 1];
    if (mes == 2 && bissexto(ano)) {
        dias = 29;
    }
    return dia <= dias;
}

bool Gasto::operator==(const Gasto& outro) const {
    return id == outro.id;
}

bool Gasto::operator<(const Gasto& outro) const {
    if (valor != outro.valor) {
        return valor < outro.valor;
    }
    return data < outro.data;
}

// O std::greater<Gasto>() do sort compara com ">", então o Gasto precisa
// dele também. "a > b" é o mesmo que "b < a".
bool Gasto::operator>(const Gasto& outro) const {
    return outro < *this;
}
