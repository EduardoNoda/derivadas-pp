#pragma once

#include "MathFunction.h"

namespace calc {

// f(x) = coefficient * x^exponent
// f'(x) = (coefficient * exponent) * x^(exponent - 1)   (regra da potencia)
// Constantes sao o caso exponent == 0: f'(x) = 0.
class PowerFunction : public MathFunction {
public:
    PowerFunction(double coefficient, double exponent)
        : coefficient_(coefficient), exponent_(exponent) {}

    [[nodiscard]] DerivativeResult derive() const override {
        return DerivativeResult{coefficient_ * exponent_, exponent_ - 1.0};
    }

private:
    double coefficient_;
    double exponent_;
};

}  // namespace calc
