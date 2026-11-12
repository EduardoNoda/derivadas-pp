#pragma once

#include <string>
#include <utility>

namespace calc {

// Descricao ESTRUTURADA de uma parcela. Independente das classes concretas
// (PowerFunction, LogarithmicFunction): e a ponte entre Parser e Factory.
// UNKNOWN: funcao reconhecida pelo nome (ex.: sin), mas sem regra de
// derivacao implementada -> o calculo inteiro deve ser cancelado.
enum class FunctionType {
    POWER,
    LOGARITHMIC,
    UNKNOWN
};

struct ParsedTerm {
    FunctionType type;
    double coefficient;    // inclui o sinal da parcela
    double exponent;       // usado por POWER; 0 para constantes e LOGARITHMIC
    std::string functionName;  // preenchido apenas quando type == UNKNOWN

    static ParsedTerm power(double coefficient, double exponent) {
        return ParsedTerm{FunctionType::POWER, coefficient, exponent, ""};
    }

    static ParsedTerm logarithmic(double coefficient) {
        return ParsedTerm{FunctionType::LOGARITHMIC, coefficient, 0.0, ""};
    }

    static ParsedTerm unknown(std::string functionName) {
        return ParsedTerm{FunctionType::UNKNOWN, 0.0, 0.0, std::move(functionName)};
    }
};

}  // namespace calc
