#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <iomanip>

using namespace std;

int main()
{
    int repeticoes = 51;
    vector<double> tempos;

    system("mkdir -p resultados_tempo/enorme");

    ofstream arquivo("resultados_tempo/enorme/tempos.csv");

    if (!arquivo) {
        cout << "Erro ao criar o arquivo de tempos.\n";
        return 1;
    }

    arquivo << "execucao,tempo_s\n";
    arquivo << fixed << setprecision(9);

    for (int i = 0; i < repeticoes; i++) {
        auto inicio = chrono::steady_clock::now();

        int resultado = system(
            "./solver enorme/f1.cnf enorme/f2_equivalente.cnf"
            " > resultados_tempo/enorme/saida.txt"
            " 2> resultados_tempo/enorme/erro.txt"
        );

        auto fim = chrono::steady_clock::now();

        if (resultado != 0) {
            cout << "Erro na execucao " << i + 1 << ".\n";
            return 1;
        }

        double tempo = chrono::duration<double>(fim - inicio).count();

        tempos.push_back(tempo);
        arquivo << i + 1 << "," << tempo << "\n";

        cout << "Execucao " << i + 1 << ": "
             << tempo << " segundos\n";
    }

    arquivo.close();

    double soma = 0;

    for (int i = 0; i < repeticoes; i++) {
        soma += tempos[i];
    }

    double media = soma / repeticoes;
    double somaQuadrados = 0;

    for (int i = 0; i < repeticoes; i++) {
        somaQuadrados += (tempos[i] - media) * (tempos[i] - media);
    }

    double desvio = sqrt(somaQuadrados / (repeticoes - 1));

    sort(tempos.begin(), tempos.end());

    // Quartis para exatamente 51 medicoes ordenadas.
    double minimo = tempos[0];
    double q1 = (tempos[12] + tempos[13]) / 2;
    double mediana = tempos[25];
    double q3 = (tempos[37] + tempos[38]) / 2;
    double maximo = tempos[50];

    ofstream resumo("resultados_tempo/enorme/resumo_tempo.txt");

    if (!resumo) {
        cout << "Erro ao criar o resumo.\n";
        return 1;
    }

    resumo << fixed << setprecision(9);
    resumo << "Repeticoes: " << repeticoes << "\n";
    resumo << "Minimo (s): " << minimo << "\n";
    resumo << "Primeiro quartil (s): " << q1 << "\n";
    resumo << "Mediana (s): " << mediana << "\n";
    resumo << "Media (s): " << media << "\n";
    resumo << "Terceiro quartil (s): " << q3 << "\n";
    resumo << "Maximo (s): " << maximo << "\n";
    resumo << "Desvio-padrao amostral (s): " << desvio << "\n";

    resumo.close();
    return 0;
}