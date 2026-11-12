#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

#include "errors/Errors.h"
#include "lexer/Lexer.h"

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

static const char* typeName(TokenType t) {
    switch (t) {
        case TokenType::Number:     return "Number";
        case TokenType::Identifier: return "Identifier";
        case TokenType::Variable:   return "Variable";
        case TokenType::Caret:      return "Caret";
        case TokenType::Plus:       return "Plus";
        case TokenType::Minus:      return "Minus";
        case TokenType::OpenParen:  return "OpenParen";
        case TokenType::CloseParen: return "CloseParen";
        case TokenType::End:        return "End";
    }
    return "?";
}

// Compara a sequencia de tipos produzida com a esperada e imprime os tokens.
static bool expectTypes(const std::vector<Token>& tokens,
                        std::initializer_list<TokenType> expected) {
    std::vector<TokenType> got;
    for (const Token& t : tokens) got.push_back(t.type);

    std::cout << "  tokens: ";
    for (const Token& t : tokens) {
        std::cout << "[" << typeName(t.type);
        if (t.type == TokenType::Number) std::cout << " " << t.numericValue;
        if (t.type == TokenType::Identifier) std::cout << " " << t.lexeme;
        std::cout << "] ";
    }
    std::cout << "\n";

    if (got.size() != expected.size()) return false;
    for (std::size_t k = 0; k < got.size(); ++k) {
        if (got[k] != *(expected.begin() + k)) return false;
    }
    return true;
}

static bool throwsSyntaxError(const std::string& input) {
    try {
        Lexer lexer;
        (void)lexer.tokenize(input);
        return false;
    } catch (const SyntaxError&) {
        return true;
    }
}

int main() {
    Lexer lexer;

    std::cout << "[1] 3x^2\n";
    CHECK(expectTypes(lexer.tokenize("3x^2"),
        {TokenType::Number, TokenType::Variable, TokenType::Caret,
         TokenType::Number, TokenType::End}));

    std::cout << "[2] 5ln(x) com espacos\n";
    {
        auto tokens = lexer.tokenize("  5ln( x ) ");
        CHECK(expectTypes(tokens,
            {TokenType::Number, TokenType::Identifier, TokenType::OpenParen,
             TokenType::Variable, TokenType::CloseParen, TokenType::End}));
        CHECK(tokens[0].numericValue == 5.0);
        CHECK(tokens[1].lexeme == "ln");
    }

    std::cout << "[3] 2x^1.5 (expoente decimal)\n";
    {
        auto tokens = lexer.tokenize("2x^1.5");
        CHECK(expectTypes(tokens,
            {TokenType::Number, TokenType::Variable, TokenType::Caret,
             TokenType::Number, TokenType::End}));
        CHECK(tokens[3].numericValue == 1.5);
    }

    std::cout << "[4] 4x^-2 (expoente negativo)\n";
    CHECK(expectTypes(lexer.tokenize("4x^-2"),
        {TokenType::Number, TokenType::Variable, TokenType::Caret,
         TokenType::Minus, TokenType::Number, TokenType::End}));

    std::cout << "[5] .5ln(x) (coeficiente decimal)\n";
    {
        auto tokens = lexer.tokenize(".5ln(x)");
        CHECK(expectTypes(tokens,
            {TokenType::Number, TokenType::Identifier, TokenType::OpenParen,
             TokenType::Variable, TokenType::CloseParen, TokenType::End}));
        CHECK(tokens[0].numericValue == 0.5);
    }

    std::cout << "[6] expressao vazia\n";
    CHECK(expectTypes(lexer.tokenize(""),
        {TokenType::End}));
    CHECK(expectTypes(lexer.tokenize("   "),
        {TokenType::End}));

    std::cout << "[7] -x^2 (sinal inicial)\n";
    CHECK(expectTypes(lexer.tokenize("-x^2"),
        {TokenType::Minus, TokenType::Variable, TokenType::Caret,
         TokenType::Number, TokenType::End}));

    std::cout << "[8] sin(x) reconhecido como identificador\n";
    {
        auto tokens = lexer.tokenize("sin(x)");
        CHECK(tokens[0].type == TokenType::Identifier);
        CHECK(tokens[0].lexeme == "sin");
    }

    // O Lexer NAO rejeita "3x^^2": os tokens sao validos um a um.
    // Quem rejeita e o Parser (erro estrutural). Testamos isso aqui:
    std::cout << "[9] 3x^^2 passa pelo Lexer (Parser rejeita depois)\n";
    CHECK(expectTypes(lexer.tokenize("3x^^2"),
        {TokenType::Number, TokenType::Variable, TokenType::Caret,
         TokenType::Caret, TokenType::Number, TokenType::End}));

    std::cout << "[10] numeros malformados e caracteres invalidos\n";
    CHECK(throwsSyntaxError("3.5.2"));
    CHECK(throwsSyntaxError("5."));
    CHECK(throwsSyntaxError("3 $ 2"));
    CHECK(throwsSyntaxError("3x^2,5"));

    if (failures == 0) {
        std::cout << "\nTODOS OS TESTES DO LEXER PASSARAM\n";
        return 0;
    }
    std::cout << "\n" << failures << " teste(s) FALHARAM\n";
    return 1;
}
