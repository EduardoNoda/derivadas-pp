#include <iostream>
#include <string>

#include "application/DerivativeService.h"

using namespace calc;

static int failures = 0;

#define CHECK(condition)                                                   \
    do {                                                                   \
        if (!(condition)) {                                                \
            std::cout << "FALHA (linha " << __LINE__ << "): "              \
                      << #condition << "\n";                                 \
            ++failures;                                                    \
        }                                                                  \
    } while (0)

static void expectOk(const DerivativeService& service,
                     const std::string& input, const std::string& expected) {
    const CalculationResult r = service.calculate(input);
    std::cout << "  f(x) = " << input << "\n  -> ok=" << r.ok
              << ", '" << r.message << "'\n";
    CHECK(r.ok);
    CHECK(r.message == expected);
}

static void expectError(const DerivativeService& service,
                        const std::string& input, const std::string& fragment) {
    const CalculationResult r = service.calculate(input);
    std::cout << "  f(x) = " << input << "\n  -> ok=" << r.ok
              << ", '" << r.message << "'\n";
    CHECK(!r.ok);
    CHECK(r.message.find(fragment) != std::string::npos);
}

int main() {
    DerivativeService service;

    std::cout << "[1] derivadas corretas\n";
    expectOk(service, "3x^2", "6x");
    expectOk(service, "5x^3 + 2x^2", "15x^2 + 4x");
    expectOk(service, "4x + 7", "4");
    expectOk(service, "5ln(x)", "5/x");
    expectOk(service, "3x^2 + 2x - 5ln(x) + 4", "6x + 2 - 5/x");
    expectOk(service, "4x^-2", "-8/x^3");
    expectOk(service, "7", "0");
    expectOk(service, "2x^1.5", "3x^0.5");

    std::cout << "[2] erros: nenhuma excecao escapa do servico\n";
    expectError(service, "", "expressao vazia");
    expectError(service, "3x^^2", "expoente");
    expectError(service, "3x^2 +", "parcela incompleta");
    expectError(service, "sin(x)", "sin(x)");
    expectError(service, "3x^2 + sin(x)", "sin(x)");  // sem derivada parcial
    expectError(service, "3 $ 2", "caractere invalido");

    if (failures == 0) {
        std::cout << "\nTODOS OS TESTES DO SERVICO PASSARAM\n";
        return 0;
    }
    std::cout << "\n" << failures << " teste(s) FALHARAM\n";
    return 1;
}
