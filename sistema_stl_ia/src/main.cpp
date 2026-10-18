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
    cout << "03. registrar gasto" << endl;
    cout << "04. listar gastos" << endl;
    cout << "05. consultar total por categoria" << endl;
    cout << "09. excluir gasto" << endl;
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

bool lerInteiro(const string& texto, int& valor) {
    istringstream stream(texto);
    stream >> valor;
    if (stream.fail()) {
        return false;
    }
    stream >> ws;
    return stream.eof();
}

// Conta caracteres de verdade (não bytes): "Alimentação" tem 11 caracteres,
// mas 13 bytes, porque ç e ã ocupam 2 bytes no UTF-8.
int contarCaracteres(const string& texto) {
    int total = 0;
    for (string::size_type i = 0; i < texto.size(); ++i) {
        unsigned char c = texto[i];
        if ((c & 0xC0) != 0x80) {
            total++;
        }
    }
    return total;
}

string nomeMes(int mes) {
    static const char* nomes[] = {"Janeiro", "Fevereiro", "Março", "Abril",
                                  "Maio", "Junho", "Julho", "Agosto",
                                  "Setembro", "Outubro", "Novembro",
                                  "Dezembro"};
    return nomes[mes - 1];
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

void opcaoRegistrarGasto(Carteira& carteira) {
    string data;
    string descricao;
    string valorTexto;
    string categoria;

    cout << "Digite a data: ";
    getline(cin, data);
    cout << "Digite a descrição: ";
    getline(cin, descricao);
    cout << "Digite o valor: ";
    getline(cin, valorTexto);
    cout << "Digite a categoria: ";
    getline(cin, categoria);

    int codigo = carteira.registrarGasto(data, descricao, valorTexto,
                                         categoria);
    if (codigo == Carteira::CATEGORIA_INEXISTENTE) {
        cout << "Categoria Inexistente!" << endl;
    } else if (codigo == Carteira::DESCRICAO_VAZIA) {
        cout << "Descrição vazia!" << endl;
    } else if (codigo == Carteira::DATA_INVALIDA) {
        cout << "Data inválida!" << endl;
    } else if (codigo == Carteira::VALOR_INVALIDO) {
        cout << "Valor inválido!" << endl;
    } else {
        cout << "Gasto registrado com o id " << carteira.ultimoId()
             << "!" << endl;
    }
}

void opcaoListarGastos(Carteira& carteira) {
    const vector<Gasto>& gastos = carteira.listarGastos();
    if (gastos.empty()) {
        cout << "Nenhum gasto registrado" << endl;
        return;
    }

    cout << fixed << setprecision(2);
    for (vector<Gasto>::size_type i = 0; i < gastos.size(); ++i) {
        const Gasto& gasto = gastos[i];
        cout << gasto.getId() << " | " << gasto.getData() << " | "
             << gasto.getDescricao() << " | R$ " << gasto.getValor()
             << " | " << gasto.getCategoria() << endl;
    }
}

void opcaoTotalPorCategoria(Carteira& carteira) {
    if (carteira.listarCategorias().empty()) {
        cout << "Nenhuma categoria registrada" << endl;
        return;
    }

    int mes = 0;
    int ano = 0;
    while (true) {
        string mesTexto;
        string anoTexto;

        cout << "Digite o mês: ";
        getline(cin, mesTexto);
        cout << "Digite o ano: ";
        getline(cin, anoTexto);
        if (!cin) {
            return;
        }

        if (lerInteiro(mesTexto, mes) && lerInteiro(anoTexto, ano)
                && mes >= 1 && mes <= 12 && ano >= 1) {
            break;
        }

        cout << "Opção inválida!" << endl;
        cout << "Deseja digitar um valor válido? (s/n): ";
        string resposta;
        getline(cin, resposta);
        if (!cin) {
            return;
        }
        if (resposta != "s" && resposta != "S") {
            return;
        }
    }

    map<string, double> totais = carteira.totaisDoMes(mes, ano);
    const map<string, Categoria>& categorias = carteira.listarCategorias();

    double soma = 0.0;
    map<string, double>::const_iterator it;
    for (it = totais.begin(); it != totais.end(); ++it) {
        soma += it->second;
    }

    cout << endl;
    cout << "Resumo de Gastos - " << nomeMes(mes) << "/" << ano << endl;
    cout << endl;
    if (soma == 0.0) {
        cout << "Nenhum gasto registrado no período selecionado" << endl;
        return;
    }

    cout << fixed << setprecision(2);
    for (it = totais.begin(); it != totais.end(); ++it) {
        int caracteres = contarCaracteres(it->first);
        cout << it->first << " ";
        for (int i = caracteres; i < 21; ++i) {
            cout << "-";
        }

        map<string, Categoria>::const_iterator achou =
            categorias.find(Categoria::normalizar(it->first));
        cout << " R$ " << left << setw(7) << it->second << right
             << "(Limite: R$ " << achou->second.getLimite() << ")" << endl;
    }
}

void opcaoExcluirGasto(Carteira& carteira) {
    string idTexto;

    cout << "Digite o id do gasto: ";
    getline(cin, idTexto);

    istringstream stream(idTexto);
    int id = 0;
    stream >> id;
    if (stream.fail() || id <= 0) {
        cout << "Nenhum gasto encontrado com esse id!" << endl;
        return;
    }
    stream >> ws;
    if (!stream.eof()) {
        cout << "Nenhum gasto encontrado com esse id!" << endl;
        return;
    }

    if (carteira.excluirGasto(id)) {
        cout << "Gasto excluído com sucesso!" << endl;
    } else {
        cout << "Nenhum gasto encontrado com esse id!" << endl;
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
        } else if (opcao == "03") {
            opcaoRegistrarGasto(carteira);
        } else if (opcao == "04") {
            opcaoListarGastos(carteira);
        } else if (opcao == "05") {
            opcaoTotalPorCategoria(carteira);
        } else if (opcao == "09") {
            opcaoExcluirGasto(carteira);
        } else if (opcao == "00") {
            cout << "Ate logo!" << endl;
            break;
        } else {
            cout << "Opção inválida!" << endl;
        }
    }

    return 0;
}
