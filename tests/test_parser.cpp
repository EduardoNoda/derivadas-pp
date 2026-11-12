#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "errors/Errors.h"
#include "lexer/Lexer.h"
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

static std::vector<ParsedTerm> parseOk(const std::string& input) {
    Lexer lexer;
    Parser parser;
    return parser.parse(lexer.tokenize(input));
}

static bool closeTo(double a, double b) {
    return std::abs(a - b) < 1e-9;
}

static bool throwsSyntax(const std::string& input) {
    try {
        parseOk(input);
        return false;
    } catch (const SyntaxError&) {
        return true;
    }
}

static bool throwsUnknownFunction(const std::string& input) {
    try {
        parseOk(input);
        return false;
    } catch (const UnknownFunctionError&) {
        return true;
    }
}

int main() {
    std::cout << "[1] 3x^2\n";
    {
        auto terms = parseOk("3x^2");
        CHECK(terms.size() == 1);
        CHECK(terms[0].type == FunctionType::POWER);
        CHECK(closeTo(terms[0].coefficient, 3.0));
        CHECK(closeTo(terms[0].exponent, 2.0));
    }

    std::cout << "[2] 5x^3 + 2x^2 (duas parcelas)\n";
    {
        auto terms = parseOk("5x^3 + 2x^2");
        CHECK(terms.size() == 2);
        CHECK(closeTo(terms[0].coefficient, 5.0) && closeTo(terms[0].exponent, 3.0));
        CHECK(closeTo(terms[1].coefficient, 2.0) && closeTo(terms[1].exponent, 2.0));
    }

    std::cout << "[3] 4x + 7 (potencia + constante)\n";
    {
        auto terms = parseOk("4x + 7");
        CHECK(terms.size() == 2);
        CHECK(terms[0].type == FunctionType::POWER);
        CHECK(closeTo(terms[0].coefficient, 4.0) && closeTo(terms[0].exponent, 1.0));
        CHECK(terms[1].type == FunctionType::POWER);
        CHECK(closeTo(terms[1].coefficient, 7.0) && closeTo(terms[1].exponent, 0.0));
    }

    std::cout << "[4] coeficientes implicitos e sinais\n";
    {
        auto t1 = parseOk("x^2");
        CHECK(closeTo(t1[0].coefficient, 1.0));
        auto t2 = parseOk("-x^2");
        CHECK(closeTo(t2[0].coefficient, -1.0));
        auto t3 = parseOk("x");
        CHECK(closeTo(t3[0].coefficient, 1.0) && closeTo(t3[0].exponent, 1.0));
        auto t4 = parseOk("-x");
        CHECK(closeTo(t4[0].coefficient, -1.0) && closeTo(t4[0].exponent, 1.0));
        auto t5 = parseOk("+3x^2");  // '+' inicial ignorado
        CHECK(closeTo(t5[0].coefficient, 3.0));
    }

    std::cout << "[5] 5ln(x), -ln(x), ln(x)\n";
    {
        auto a = parseOk("5ln(x)");
        CHECK(a[0].type == FunctionType::LOGARITHMIC && closeTo(a[0].coefficient, 5.0));
        auto b = parseOk("-ln(x)");
        CHECK(b[0].type == FunctionType::LOGARITHMIC && closeTo(b[0].coefficient, -1.0));
        auto c = parseOk("ln(x)");
        CHECK(c[0].type == FunctionType::LOGARITHMIC && closeTo(c[0].coefficient, 1.0));
    }

    std::cout << "[6] expressao completa: 3x^2 + 2x - 5ln(x) + 4\n";
    {
        auto terms = parseOk("3x^2 + 2x - 5ln(x) + 4");
        CHECK(terms.size() == 4);
        CHECK(closeTo(terms[0].coefficient, 3.0) && closeTo(terms[0].exponent, 2.0));
        CHECK(closeTo(terms[1].coefficient, 2.0) && closeTo(terms[1].exponent, 1.0));
        CHECK(terms[2].type == FunctionType::LOGARITHMIC && closeTo(terms[2].coefficient, -5.0));
        CHECK(closeTo(terms[3].coefficient, 4.0) && closeTo(terms[3].exponent, 0.0));
    }

    std::cout << "[7] expoentes negativos e decimais\n";
    {
        auto a = parseOk("4x^-2");
        CHECK(closeTo(a[0].exponent, -2.0));
        auto b = parseOk("2x^1.5");
        CHECK(closeTo(b[0].exponent, 1.5));
    }

    std::cout << "[8] constantes isoladas\n";
    {
        auto a = parseOk("2.5");
        CHECK(closeTo(a[0].coefficient, 2.5) && closeTo(a[0].exponent, 0.0));
        auto b = parseOk("-5");
        CHECK(closeTo(b[0].coefficient, -5.0) && closeTo(b[0].exponent, 0.0));
        auto c = parseOk("0");
        CHECK(closeTo(c[0].coefficient, 0.0));
    }

    std::cout << "[9] erros sintaticos\n";
    CHECK(throwsSyntax(""));            // vazio
    CHECK(throwsSyntax("-"));          // so sinal
    CHECK(throwsSyntax("3x^2 +"));     // operador sobrando
    CHECK(throwsSyntax("3x^^2"));      // potencia sem expoente
    CHECK(throwsSyntax("3x^2 5"));     // operador faltando
    CHECK(throwsSyntax("3x^2 + +2"));  // sinal duplicado
    CHECK(throwsSyntax("ln x"));       // parentese faltando
    CHECK(throwsSyntax("ln(x^2)"));    // so ln(x) na notacao rigida
    CHECK(throwsSyntax("()"));         // parcela vazia
    CHECK(throwsSyntax("x^--2"));      // sinal duplo no expoente
    CHECK(throwsSyntax("x^+2"));       // gramatica rigida: so '-' no expoente

    std::cout << "[10] funcao sem implementacao: calculo cancelado\n";
    CHECK(throwsUnknownFunction("sin(x)"));
    CHECK(throwsUnknownFunction("3x^2 + sin(x)"));   // NAO pode derivar parcial
    CHECK(throwsUnknownFunction("cos(x)"));

    if (failures == 0) {
        std::cout << "\nTODOS OS TESTES DO PARSER PASSARAM\n";
        return 0;
    }
    std::cout << "\n" << failures << " teste(s) FALHARAM\n";
    return 1;
}
