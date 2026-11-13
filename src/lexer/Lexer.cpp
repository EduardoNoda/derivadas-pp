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
            tokens.push_back(Token::make(TokenType::Number, lexeme, std::stod(lexeme), start + 1));
            continue;
        }

        if (isLetter(c)) {
            const std::size_t start = i;
            while (i < input.size() && isLetter(input[i])) {
                ++i;
            }
            const std::string lexeme = input.substr(start, i - start);
            if (lexeme == "x") {
                tokens.push_back(Token::make(TokenType::Variable, lexeme, 0.0, start + 1));
            } else {
                tokens.push_back(Token::make(TokenType::Identifier, lexeme, 0.0, start + 1));
            }
            continue;
        }

        const std::size_t pos = i + 1;
        switch (c) {
        case '^': tokens.push_back(Token::make(TokenType::Caret, "^", 0.0, pos)); break;
        case '+': tokens.push_back(Token::make(TokenType::Plus, "+", 0.0, pos)); break;
        case '-': tokens.push_back(Token::make(TokenType::Minus, "-", 0.0, pos)); break;
        case '(': tokens.push_back(Token::make(TokenType::OpenParen, "(", 0.0, pos)); break;
        case ')': tokens.push_back(Token::make(TokenType::CloseParen, ")", 0.0, pos)); break;
        case '/':
            throw SyntaxError(
                "Erro sintatico: o operador '/' nao e suportado (posicao " +
                std::to_string(pos) + "). Escreva a divisao como expoente "
                "negativo: 1/x = x^-1, 3/x^2 = 3x^-2.");
        default:
            fail(std::string("caractere invalido '") + c + "'");
        }
        ++i;
    }

    tokens.push_back(Token::make(TokenType::End, "", 0.0, input.size()));
    return tokens;
}

}  // namespace calc
