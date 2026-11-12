#pragma once

#include <vector>

#include "../lexer/Token.h"
#include "ParsedTerm.h"

namespace calc {

// Analisador sintatico: consome os tokens do Lexer segundo a gramatica
// aprovada e produz a lista estruturada de parcelas (ParsedTerm).
//
// Gramatica implementada:
//   expressao   := parcela (('+' | '-') parcela)*
//   parcela     := ['+' | '-'] elemento        ('+' inicial e ignorado)
//   elemento    := potencia | logaritmo | constante
//   potencia    := coeficiente 'x' ['^' expoente]
//   logaritmo   := coeficiente 'ln' '(' 'x' ')'
//   coeficiente := numero | vazio            (vazio = 1, ou -1 com sinal)
//   expoente    := ['-'] numero
//   constante   := numero                    (equivale a numero * x^0)
//
// Lanca SyntaxError para estrutura invalida e UnknownFunctionError quando
// uma funcao reconhecida pelo nome nao possui regra implementada.
// Nao implementa regras de derivacao e nao depende de SFML.
class Parser {
public:
    [[nodiscard]] std::vector<ParsedTerm> parse(const std::vector<Token>& tokens) const;

private:
    class Cursor;  // iterador interno sobre os tokens

    ParsedTerm parseTerm(Cursor& cursor, double sign) const;
    double parseExponent(Cursor& cursor) const;
    void expect(Cursor& cursor, TokenType type, const char* message) const;
};

}  // namespace calc
