// Interface de TERMINAL: demonstra que o nucleo (DerivativeService) e
// totalmente independente da interface grafica. Nao contem regra matematica
// nenhuma; apenas le a entrada, chama o servico e exibe o resultado.
//
// Este arquivo NAO faz parte do escopo obrigatorio de entrega: serve para
// validar o isolamento do nucleo. Pode ser removido do pacote final.
#include <iostream>
#include <string>

#include "application/DerivativeService.h"

int main() {
    calc::DerivativeService service;

    std::cout << "Calculadora simbolica de derivadas (modo texto)\n"
              << "Digite a funcao em x, sem o prefixo f(x) =.\n"
              << "Exemplos: 3x^2 + 5ln(x)   |   4x^-2\n"
              << "Digite 'sair' para encerrar.\n\n";

    std::string line;
    while (true) {
        std::cout << "f(x) = ";
        if (!std::getline(std::cin, line)) {
            break;  // fim da entrada (Ctrl+Z no Windows, Ctrl+D no Linux)
        }
        if (line == "sair") {
            break;
        }

        const calc::CalculationResult result = service.calculate(line);
        if (result.ok) {
            std::cout << "f'(x) = " << result.message << "\n\n";
        } else {
            std::cout << result.message << "\n\n";
        }
    }
    return 0;
}
