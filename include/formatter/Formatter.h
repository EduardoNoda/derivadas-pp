#pragma once

#include <string>
#include <vector>

#include "../math/DerivativeResult.h"

namespace calc {

class Formatter {
public:
    [[nodiscard]] std::string format(const std::vector<DerivativeResult>& results) const;

private:
    [[nodiscard]] static std::string formatTerm(double coefficient, double exponent);
    [[nodiscard]] static std::string formatNumber(double value);
};

}  // namespace calc
