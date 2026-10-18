#ifndef CATEGORIA_HPP
#define CATEGORIA_HPP

#include <string>

using namespace std;

class Categoria {
public:
    Categoria();
    Categoria(string nome, double limite);

    string getNome() const;
    double getLimite() const;
    void setLimite(double limite);

    static string normalizar(const string& texto);

private:
    string nome;
    double limite;
};

#endif
