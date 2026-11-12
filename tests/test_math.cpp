#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "math/LogarithmicFunction.h"
#include "math/MathFunction.h"
#include "math/PowerFunction.h"

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

static bool closeTo(double a, double b) {
    return std::abs(a - b) < 1e-9;
}

// Verifica derive() e imprime o resultado estruturado.
static bool expectDerive(const MathFunction& f, double coef, double exp) {
    DerivativeResult r = f.derive();
    std::cout << "  -> coeficiente " << r.coefficient
              << ", expoente " << r.exponent << "\n";
    return closeTo(r.coefficient, coef) && closeTo(r.exponent, exp);
}

int main() {
    std::cout << "[1] regra da potencia\n";
    CHECK(expectDerive(PowerFunction(3, 2), 6.0, 1.0));     // 3x^2  -> 6x
    CHECK(expectDerive(PowerFunction(5, 3), 15.0, 2.0));    // 5x^3  -> 15x^2
    CHECK(expectDerive(PowerFunction(2, 1), 2.0, 0.0));     // 2x    -> 2
    CHECK(expectDerive(PowerFunction(4, -2), -8.0, -3.0));  // 4x^-2 -> -8x^-3

    std::cout << "[2] casos especiais da potencia\n";
    CHECK(expectDerive(PowerFunction(1, 2), 2.0, 1.0));     // x^2   -> 2x
    CHECK(expectDerive(PowerFunction(-1, 2), -2.0, 1.0));   // -x^2  -> -2x
    CHECK(expectDerive(PowerFunction(0, 5), 0.0, 4.0));     // 0x^5  -> 0
    CHECK(expectDerive(PowerFunction(7, 0), 0.0, -1.0));    // 7     -> 0
    CHECK(expectDerive(PowerFunction(2, 1.5), 3.0, 0.5));   // 2x^1.5-> 3x^0.5
    CHECK(expectDerive(PowerFunction(-5, 0), 0.0, -1.0));   // -5    -> 0

    std::cout << "[3] regra do logaritmo\n";
    CHECK(expectDerive(LogarithmicFunction(5), 5.0, -1.0));    // 5ln(x)  -> 5/x
    CHECK(expectDerive(LogarithmicFunction(-1), -1.0, -1.0));  // -ln(x)  -> -1/x
    CHECK(expectDerive(LogarithmicFunction(0.5), 0.5, -1.0));  // 0.5ln(x)-> 0.5/x
    CHECK(expectDerive(LogarithmicFunction(0), 0.0, -1.0));    // 0ln(x)  -> 0

    std::cout << "[4] polimorfismo: vetor de MathFunction\n";
    {
        std::vector<std::unique_ptr<MathFunction>> funcs;
        funcs.push_back(std::make_unique<PowerFunction>(3, 2));
        funcs.push_back(std::make_unique<LogarithmicFunction>(-5));
        funcs.push_back(std::make_unique<PowerFunction>(4, 0));

        std::vector<DerivativeResult> results;
        for (const auto& f : funcs) {
            results.push_back(f->derive());  // chamada virtual: sem saber o tipo concreto
        }
        CHECK(closeTo(results[0].coefficient, 6.0) && closeTo(results[0].exponent, 1.0));
        CHECK(closeTo(results[1].coefficient, -5.0) && closeTo(results[1].exponent, -1.0));
        CHECK(closeTo(results[2].coefficient, 0.0));
    }

    if (failures == 0) {
        std::cout << "\nTODOS OS TESTES DO MODELO MATEMATICO PASSARAM\n";
        return 0;
    }
    std::cout << "\n" << failures << " teste(s) FALHARAM\n";
    return 1;
}
