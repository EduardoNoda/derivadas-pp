#pragma once

#include <memory>

#include "../math/MathFunction.h"
#include "../parser/ParsedTerm.h"

namespace calc {

// Ponto central de CRIACAO das funcoes matematicas.
// Recebe a descricao estruturada do Parser (ParsedTerm) e devolve o
// objeto polimorfico correspondente.
//
// Conforme exigencia do enunciado, a Factory NAO contem:
//   - as regras de derivacao (elas vivem nas classes de math/)
//   - o analisador lexico (ele vive em lexer/)
// Ela apenas instancia a classe concreta certa para cada tipo.
//
// Para adicionar uma nova funcao derivavel (ex.: seno), basta:
//   1. criar a classe concreta em math/ (ex.: SineFunction);
//   2. adicionar um valor em FunctionType (parser/ParsedTerm.h);
//   3. adicionar um case neste switch.
// Nenhuma classe existente precisa ser reescrita.
class FunctionFactory {
public:
    [[nodiscard]] static std::unique_ptr<MathFunction> create(const ParsedTerm& term);
};

}  // namespace calc
