#include "Categoria.hpp"

using namespace std;

Categoria::Categoria() : limite(0.0) {
}

Categoria::Categoria(string nome, double limite)
    : nome(nome), limite(limite) {
}

string Categoria::getNome() const {
    return nome;
}

double Categoria::getLimite() const {
    return limite;
}

void Categoria::setLimite(double limite) {
    this->limite = limite;
}

string Categoria::normalizar(const string& texto) {
    string saida;
    for (string::size_type i = 0; i < texto.size(); ++i) {
        unsigned char c = texto[i];
        if (c < 128) {
            if (c >= 'A' && c <= 'Z') {
                saida += c + ('a' - 'A');
            } else {
                saida += c;
            }
        } else if (c == 0xC3 && i + 1 < texto.size()) {
            unsigned char segundo = texto[i + 1];
            i++;
            switch (segundo) {
                case 0x80: case 0xA0:
                case 0x81: case 0xA1:
                case 0x82: case 0xA2:
                case 0x83: case 0xA3:
                    saida += 'a';
                    break;
                case 0x87: case 0xA7:
                    saida += 'c';
                    break;
                case 0x89: case 0xA9:
                case 0x8A: case 0xAA:
                    saida += 'e';
                    break;
                case 0x8D: case 0xAD:
                    saida += 'i';
                    break;
                case 0x93: case 0xB3:
                case 0x94: case 0xB4:
                case 0x95: case 0xB5:
                    saida += 'o';
                    break;
                case 0x9A: case 0xBA:
                    saida += 'u';
                    break;
                default:
                    break;
            }
        }
    }
    return saida;
}
