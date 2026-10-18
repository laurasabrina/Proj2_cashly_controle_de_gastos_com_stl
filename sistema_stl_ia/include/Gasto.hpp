#ifndef GASTO_HPP
#define GASTO_HPP

#include <string>

using namespace std;

class Gasto {
public:
    Gasto();
    Gasto(int id);
    Gasto(int id, string data, string descricao, double valor,
          string categoria);

    int getId() const;
    string getData() const;
    string getDescricao() const;
    double getValor() const;
    string getCategoria() const;

    static bool dataValida(const string& data);

    bool operator==(const Gasto& outro) const;
    bool operator<(const Gasto& outro) const;

private:
    int id;
    string data;
    string descricao;
    double valor;
    string categoria;
};

#endif
