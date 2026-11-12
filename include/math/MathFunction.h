#pragma once

#include "DerivativeResult.h"

namespace calc {

// Abstracao comum de uma funcao matematica de uma variavel.
// Cada classe concreta (PowerFunction, LogarithmicFunction, futuras)
// encapsula a SUA regra de derivacao. E aqui que mora o polimorfismo
// que permite estender o programa sem reescrever as classes existentes.
class MathFunction {
public:
    virtual ~MathFunction() = default;

    // Regra de derivacao da funcao concreta.
    [[nodiscard]] virtual DerivativeResult derive() const = 0;
};

}  // namespace calc
