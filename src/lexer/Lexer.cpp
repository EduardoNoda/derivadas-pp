#include "../../include/lexer/Lexer.h"

#include <cctype>

#include "../../include/errors/Errors.h"

namespace calc {

namespace {

bool isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool isLetter(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

}  // namespace

std::vector<Token> Lexer::tokenize(const std::string& input) const {
    std::vector<Token> tokens;
    std::size_t i = 0;

    auto fail = [&](const std::string& reason) -> void {
        throw SyntaxError("Erro sintatico: " + reason +
                          " na posicao " + std::to_string(i + 1) + ".");
    };

    while (i < input.size()) {
        const char c = input[i];

        if (std::isspace(static_cast<unsigned char>(c)) != 0) {
            ++i;
            continue;
        }

        if (isDigit(c) || (c == '.' && i + 1 < input.size() && isDigit(input[i + 1]))) {
            const std::size_t start = i;
            bool hasDot = false;
            while (i < input.size() && (isDigit(input[i]) || input[i] == '.')) {
                if (input[i] == '.') {
                    if (hasDot) {
                        fail("numero com mais de um ponto decimal");
                    }
                    hasDot = true;
                }
                ++i;
            }
            
            if (input[i - 1] == '.') {
                fail("numero decimal incompleto");
            }
            const std::string lexeme = input.substr(start, i - start);
            tokens.push_back(Token::make(TokenType::Number, lexeme, std::stod(lexeme)));
            continue;
        }

        if (isLetter(c)) {
            const std::size_t start = i;
            while (i < input.size() && isLetter(input[i])) {
                ++i;
            }
            const std::string lexeme = input.substr(start, i - start);
            if (lexeme == "x") {
                tokens.push_back(Token::make(TokenType::Variable, lexeme));
            } else {
                tokens.push_back(Token::make(TokenType::Identifier, lexeme));
            }
            continue;
        }

        switch (c) {
            case '^': tokens.push_back(Token::make(TokenType::Caret, "^")); break;
            case '+': tokens.push_back(Token::make(TokenType::Plus, "+")); break;
            case '-': tokens.push_back(Token::make(TokenType::Minus, "-")); break;
            case '(': tokens.push_back(Token::make(TokenType::OpenParen, "(")); break;
            case ')': tokens.push_back(Token::make(TokenType::CloseParen, ")")); break;
            default:
                fail(std::string("caractere invalido '") + c + "'");
        }
        ++i;
    }

    tokens.push_back(Token::make(TokenType::End, ""));
    return tokens;
}

}  // namespace calc
