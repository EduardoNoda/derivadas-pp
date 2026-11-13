#include "../../include/parser/Parser.h"

#include <algorithm>

#include "../../include/errors/Errors.h"

namespace calc {
class Parser::Cursor {
public:
    explicit Cursor(const std::vector<Token>& tokens) : tokens_(tokens) {}

    const Token& peek() const { return tokens_[index_]; }

    const Token& advance() {
        const Token& current = tokens_[index_];
        if (tokens_[index_].type != TokenType::End) {
            ++index_;
        }
        return current;
    }

    bool atEnd() const { return peek().type == TokenType::End; }

    std::size_t position() const { return peek().position; }

private:
    const std::vector<Token>& tokens_;
    std::size_t index_ = 0;
};

std::vector<ParsedTerm> Parser::parse(const std::vector<Token>& tokens) const {
    Cursor cursor(tokens);

    if (cursor.atEnd()) {
        throw SyntaxError("Erro sintatico: expressao vazia.");
    }

    std::vector<ParsedTerm> terms;

    double firstSign = 1.0;
    if (cursor.peek().type == TokenType::Plus) {
        cursor.advance();
    } else if (cursor.peek().type == TokenType::Minus) {
        cursor.advance();
        firstSign = -1.0;
    }
    terms.push_back(parseTerm(cursor, firstSign));

    while (!cursor.atEnd()) {
        const TokenType t = cursor.peek().type;
        if (t != TokenType::Plus && t != TokenType::Minus) {
            throw SyntaxError("Erro sintatico: era esperado '+' ou '-' na posicao " +
                              std::to_string(cursor.position()) + ".");
        }
        const double sign = (cursor.advance().type == TokenType::Minus) ? -1.0 : 1.0;
        terms.push_back(parseTerm(cursor, sign));
    }

    return terms;
}

ParsedTerm Parser::parseTerm(Cursor& cursor, double sign) const {
    double coefficient = 1.0;  // coeficiente implicito
    bool hasNumber = false;

    if (cursor.peek().type == TokenType::Number) {
        coefficient = cursor.advance().numericValue;
        hasNumber = true;
    }

    ParsedTerm term = ParsedTerm::power(1.0, 0.0);  // valor so para inicializar
    const TokenType next = cursor.peek().type;

    if (next == TokenType::Variable) {
        cursor.advance();
        double exponent = 1.0;
        if (cursor.peek().type == TokenType::Caret) {
            cursor.advance();
            exponent = parseExponent(cursor);
        }
        term = ParsedTerm::power(coefficient, exponent);
    } else if (next == TokenType::Identifier) {
        const std::string name = cursor.advance().lexeme;
        if (cursor.peek().type != TokenType::OpenParen) {
            // Ex.: "lnx", "sinx": funcoes exigem parenteses na notacao rigida.
            if (name == "ln") {
                throw SyntaxError("Erro sintatico: era esperado '(' apos 'ln' na posicao " +
                    std::to_string(cursor.position()) +
                    ". Escreva ln(x), com parenteses.");
            }
            std::string message = "Erro sintatico: '" + name + "' nao e reconhecido. "
                "Funcoes exigem parenteses, ex.: ln(x); "
                "a variavel e apenas 'x'.";
            if (name.rfind("ln", 0) == 0) {
                message += " Voce quis dizer ln(x)?";
            }
            throw SyntaxError(message);
        }
        if (name != "ln") {
            throw UnknownFunctionError(name);
        }
        expect(cursor, TokenType::OpenParen, "era esperado '(' apos 'ln'");
        expect(cursor, TokenType::Variable, "era esperado 'x' dentro de ln(...)");
        expect(cursor, TokenType::CloseParen, "era esperado ')' apos 'x'");
        term = ParsedTerm::logarithmic(coefficient);
    } else if (hasNumber && next == TokenType::Caret) {
        throw SyntaxError("Erro sintatico: o expoente so pode ser aplicado a variavel x "
        "(posicao " + std::to_string(cursor.position()) +
        "). Escreva 10x^3, por exemplo, para uma potencia de x; para uma "
        "constante, digite apenas o numero.");
    } else if (hasNumber) {
        term = ParsedTerm::power(coefficient, 0.0);
    } else {
        throw SyntaxError("Erro sintatico: parcela incompleta na posicao " +
                          std::to_string(cursor.position()) + ".");
    }

    term.coefficient *= sign;
    return term;
}

double Parser::parseExponent(Cursor& cursor) const {
    double sign = 1.0;
    if (cursor.peek().type == TokenType::Minus) {
        cursor.advance();
        sign = -1.0;
    }
    if (cursor.peek().type != TokenType::Number) {
        throw SyntaxError("Erro sintatico: era esperado um numero como expoente "
                          "na posicao " + std::to_string(cursor.position()) + ".");
    }
    return sign * cursor.advance().numericValue;
}

void Parser::expect(Cursor& cursor, TokenType type, const char* message) const {
    if (cursor.peek().type != type) {
        throw SyntaxError("Erro sintatico: " + std::string(message) +
                          " na posicao " + std::to_string(cursor.position()) + ".");
    }
    cursor.advance();
}

}  // namespace calc
