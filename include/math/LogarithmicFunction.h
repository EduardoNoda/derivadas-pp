#pragma once

#include "MathFunction.h"

namespace calc {

// f(x) = coefficient * ln(x)
// f'(x) = coefficient / x, representado como coefficient * x^-1
//         (o Formatter decide a forma de exibicao).
class LogarithmicFunction : public MathFunction {
public:
    explicit LogarithmicFunction(double coefficient)
        : coefficient_(coefficient) {}

    [[nodiscard]] DerivativeResult derive() const override {
        return DerivativeResult{coefficient_, -1.0};
    }

private:
    double coefficient_;
};

}  // namespace calc
