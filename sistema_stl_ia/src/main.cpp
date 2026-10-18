#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Carteira.hpp"

using namespace std;

void mostrarMenu() {
    cout << "===== Cashly =====" << endl;
    cout << "01. cadastrar categoria" << endl;
    cout << "02. listar categorias" << endl;
    cout << "00. sair" << endl;
    cout << "Escolha uma opcao: ";
}

bool lerDouble(const string& texto, double& valor) {
    istringstream stream(texto);
    stream >> valor;
    if (stream.fail()) {
        return false;
    }
    stream >> ws;
    return stream.eof();
}

void opcaoCadastrarCategoria(Carteira& carteira) {
    string nome;
    string limiteTexto;

    cout << "Digite o nome da categoria: ";
    getline(cin, nome);
    cout << "Digite o limite mensal: ";
    getline(cin, limiteTexto);

    double limite = 0.0;
    if (!lerDouble(limiteTexto, limite)) {
        cout << "Limite inválido!" << endl;
        return;
    }

    int codigo = carteira.cadastrarCategoria(nome, limite);
    if (codigo == Carteira::NOME_VAZIO) {
        cout << "Nome de categoria vazio!" << endl;
    } else if (codigo == Carteira::LIMITE_INVALIDO) {
        cout << "Limite inválido!" << endl;
    } else if (codigo == Carteira::CATEGORIA_DUPLICADA) {
        cout << "Categoria já cadastrada!" << endl;
    } else {
        cout << "Categoria cadastrada com sucesso!" << endl;
    }
}

void opcaoListarCategorias(Carteira& carteira) {
    const map<string, Categoria>& categorias = carteira.listarCategorias();
    if (categorias.empty()) {
        cout << "Nenhuma categoria registrada" << endl;
        return;
    }

    cout << "Categorias cadastradas:" << endl;
    map<string, Categoria>::const_iterator it;
    for (it = categorias.begin(); it != categorias.end(); ++it) {
        cout << fixed << setprecision(2);
        cout << it->second.getNome() << " - Limite: R$ "
             << it->second.getLimite() << endl;
    }
}

int main() {
    Carteira carteira;
    string opcao;

    while (true) {
        mostrarMenu();
        getline(cin, opcao);
        if (!cin) {
            break;
        }
        if (opcao.size() == 1) {
            opcao = "0" + opcao;
        }

        if (opcao == "01") {
            opcaoCadastrarCategoria(carteira);
        } else if (opcao == "02") {
            opcaoListarCategorias(carteira);
        } else if (opcao == "00") {
            cout << "Ate logo!" << endl;
            break;
        } else {
            cout << "Opção inválida!" << endl;
        }
    }

    return 0;
}
