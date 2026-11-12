#pragma once

namespace calc {

// Resultado estruturado de uma derivada: coeficiente * x^exponente.
// Essa representacao cobre todos os casos do escopo:
//   - derivada de a*x^y   -> (a*y) * x^(y-1)
//   - derivada de a*ln(x) -> a * x^-1        (formatada como a/x)
//   - derivada de constante -> coeficiente 0 (parcela omitida no formato)
struct DerivativeResult {
    double coefficient;
    double exponent;
};

}  // namespace calc
