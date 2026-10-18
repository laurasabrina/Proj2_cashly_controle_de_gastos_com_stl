#include <iostream>

#include "Categoria.hpp"
#include "Gasto.hpp"

using namespace std;

int main() {
    cout << "--- Teste dataValida ---" << endl;
    cout << "29/02/2024: " << Gasto::dataValida("29/02/2024") << endl;
    cout << "31/02/2026: " << Gasto::dataValida("31/02/2026") << endl;
    cout << "12/10/2026: " << Gasto::dataValida("12/10/2026") << endl;
    cout << "abc:        " << Gasto::dataValida("abc") << endl;
    cout << "32/01/2026: " << Gasto::dataValida("32/01/2026") << endl;
    cout << "15/13/2026: " << Gasto::dataValida("15/13/2026") << endl;

    cout << endl << "--- Teste normalizar ---" << endl;
    cout << "Alimentacao -> " << Categoria::normalizar("Alimentação") << endl;
    cout << "ALIMENTAÇÃO -> " << Categoria::normalizar("ALIMENTAÇÃO") << endl;
    cout << "Saúde -> " << Categoria::normalizar("Saúde") << endl;

    cout << endl << "--- Teste operadores do Gasto ---" << endl;
    Gasto g1(1, "12/10/2026", "almoço", 45.90, "Alimentação");
    Gasto g2(2, "15/10/2026", "mercado", 200.00, "Alimentação");
    Gasto g3(3, "14/10/2026", "combustível", 45.90, "Transporte");
    Gasto g1Copia(1);

    cout << "g1 == g1Copia (mesmo id): " << (g1 == g1Copia) << endl;
    cout << "g1 == g2 (ids diferentes): " << (g1 == g2) << endl;
    cout << "g1 < g2 (45.90 < 200.00): " << (g1 < g2) << endl;
    cout << "g2 < g1: " << (g2 < g1) << endl;
    cout << "g3 < g1 (mesmo valor, g3 mais recente): " << (g3 < g1) << endl;

    cout << endl << "--- Teste getters ---" << endl;
    cout << "g2 id=" << g2.getId() << " data=" << g2.getData()
         << " descricao=" << g2.getDescricao()
         << " valor=" << g2.getValor()
         << " categoria=" << g2.getCategoria() << endl;

    cout << endl << "--- Teste Categoria ---" << endl;
    Categoria cat("Alimentação", 500.00);
    cout << "nome=" << cat.getNome() << " limite=" << cat.getLimite() << endl;
    cat.setLimite(600.00);
    cout << "depois de setLimite(600): limite=" << cat.getLimite() << endl;

    return 0;
}
