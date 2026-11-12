#include <iostream>
#include <string>
#include <vector>

#include "errors/Errors.h"
#include "factory/FunctionFactory.h"
#include "formatter/Formatter.h"
#include "lexer/Lexer.h"
#include "math/MathFunction.h"
#include "parser/Parser.h"

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

// Fluxo completo: Lexer -> Parser -> Factory -> derive -> Formatter.
static std::string differentiate(const std::string& input) {
    Lexer lexer;
    Parser parser;
    const auto terms = parser.parse(lexer.tokenize(input));

    std::vector<DerivativeResult> results;
    for (const ParsedTerm& term : terms) {
        auto function = FunctionFactory::create(term);
        results.push_back(function->derive());
    }

    Formatter formatter;
    return formatter.format(results);
}

static void expect(const std::string& input, const std::string& expected) {
    const std::string got = differentiate(input);
    std::cout << "  f(x) = " << input << "\n  f'(x) = " << got
              << "   (esperado: " << expected << ")\n";
    CHECK(got == expected);
}

int main() {
    std::cout << "[1] casos obrigatorios da tabela\n";
    expect("3x^2", "6x");
    expect("5x^3 + 2x^2", "15x^2 + 4x");
    expect("4x + 7", "4");
    expect("5ln(x)", "5/x");
    expect("3x^2 + 2x - 5ln(x) + 4", "6x + 2 - 5/x");
    expect("4x^-2", "-8/x^3");
    expect("7", "0");

    std::cout << "[2] casos especiais\n";
    expect("x", "1");
    expect("-x^2", "-2x");
    expect("x^2", "2x");
    expect("0x^2", "0");
    expect("ln(x)", "1/x");
    expect("-ln(x)", "-1/x");
    expect("3x^-1", "-3/x^2");  // 3*(-1)x^-2 = -3/x^2
    expect("2x^1.5", "3x^0.5");
    expect("x^-2", "-2/x^3");
    expect("7 - 3x^2 + 5ln(x)", "-6x + 5/x");
    expect("5", "0");
    expect("0", "0");
    expect("-5", "0");

    std::cout << "[3] Factory defensiva: UNKNOWN cancela o calculo\n";
    {
        bool caught = false;
        try {
            (void)FunctionFactory::create(ParsedTerm::unknown("sin"));
        } catch (const UnknownFunctionError&) {
            caught = true;
        }
        CHECK(caught);
    }

    if (failures == 0) {
        std::cout << "\nTODOS OS TESTES DO PIPELINE PASSARAM\n";
        return 0;
    }
    std::cout << "\n" << failures << " teste(s) FALHARAM\n";
    return 1;
}
