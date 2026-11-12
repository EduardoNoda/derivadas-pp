#pragma once

#include <string>

#include "../factory/FunctionFactory.h"
#include "../formatter/Formatter.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"

namespace calc {

// Resultado de um calculo: ok=true  -> message contem a derivada formatada
//                          ok=false -> message contem a mensagem de erro
struct CalculationResult {
    bool ok;
    std::string message;
};

// Ponto de entrada UNICO do nucleo, independente de qualquer interface.
// Orquestra o fluxo: Lexer -> Parser -> Factory -> derive -> Formatter.
// Captura as excecoes do nucleo, de modo que nenhuma interface (SFML,
// terminal, outra) precise conhecer SyntaxError ou UnknownFunctionError.
//
// Uso (identico para qualquer interface):
//   DerivativeService service;
//   CalculationResult r = service.calculate("3x^2 + 5ln(x)");
//   if (r.ok) { /* exibir "f'(x) = " + r.message */ }
//   else      { /* exibir r.message como erro     */ }
class DerivativeService {
public:
    [[nodiscard]] CalculationResult calculate(const std::string& expression) const;

private:
    // Componentes stateless do nucleo (sem SFML, sem regras na factory).
    Lexer lexer_;
    Parser parser_;
    Formatter formatter_;
    // FunctionFactory e estatica: apenas cria objetos a partir de ParsedTerm.
};

}  // namespace calc
