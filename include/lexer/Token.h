#pragma once

#include <cstddef>
#include <string>
#include <utility>

namespace calc {

enum class TokenType {
    Number,
    Identifier,
    Variable,
    Caret,
    Plus,
    Minus,
    OpenParen,
    CloseParen,
    End
};

struct Token {
    TokenType type;
    std::string lexeme;
    double numericValue;
    std::size_t position;;

    static Token make(TokenType type, std::string lexeme, double value = 0.0,
                        std::size_t position = 0) {
        return Token{type, std::move(lexeme), value, position};
    }
};

}  // namespace calc
