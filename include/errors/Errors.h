#pragma once

#include <stdexcept>
#include <string>

namespace calc {

// Erro na estrutura da entrada (caractere invalido, numero malformado,
// expressao fora da gramatica). Usado pelo Lexer e pelo Parser.
class SyntaxError : public std::runtime_error {
public:
    explicit SyntaxError(const std::string& message)
        : std::runtime_error(message) {}
};

// A funcao e reconhecida pelo nome, mas nao possui regra de derivacao
// implementada. O calculo inteiro deve ser cancelado (requisito do enunciado).
class UnknownFunctionError : public std::runtime_error {
public:
    explicit UnknownFunctionError(const std::string& functionName)
        : std::runtime_error(
              "Erro: nao existe uma regra de derivacao implementada "
              "para a funcao " + functionName + "(x). O calculo foi cancelado.") {}
};

}  // namespace calc
