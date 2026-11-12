#pragma once

#include <string>
#include <vector>

#include "Token.h"

namespace calc {

// Analisador lexico: le a string de entrada e a divide em tokens.
// Nao conhece matematica, nao cria objetos de funcao e nao depende de SFML.
// Lanca SyntaxError em caso de caractere invalido ou numero malformado.
class Lexer {
public:
    [[nodiscard]] std::vector<Token> tokenize(const std::string& input) const;
};

}  // namespace calc
