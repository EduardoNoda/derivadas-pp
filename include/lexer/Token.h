#pragma once

#include <string>
#include <utility>

namespace calc {

// Categorias fechadas: enum class conforme combinado na arquitetura.
enum class TokenType {
    Number,      // 3, 0.5, .25, -4 (o sinal vem como token separado)
    Identifier,  // nome de funcao: ln, sin, cos... (reconhecido pelo nome)
    Variable,    // x
    Caret,       // ^
    Plus,        // +
    Minus,       // -
    OpenParen,   // (
    CloseParen,  // )
    End          // fim da entrada
};

struct Token {
    TokenType type;
    std::string lexeme;    // texto original, util para mensagens de erro
    double numericValue;   // valido apenas quando type == Number

    static Token make(TokenType type, std::string lexeme, double value = 0.0) {
        return Token{type, std::move(lexeme), value};
    }
};

}  // namespace calc
