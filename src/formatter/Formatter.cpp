#include "../../include/formatter/Formatter.h"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace calc {

std::string Formatter::format(const std::vector<DerivativeResult>& results) const {
    std::string output;
    bool first = true;

    for (const DerivativeResult& r : results) {
        if (r.coefficient == 0.0) {
            continue; 
        }

        const double magnitude = std::abs(r.coefficient);

        if (first) {
            if (r.coefficient < 0.0) {
                output += "-";
            }
            first = false;
        } else {
            output += (r.coefficient < 0.0) ? " - " : " + ";
        }

        output += formatTerm(magnitude, r.exponent);
    }

    return output.empty() ? "0" : output;
}

std::string Formatter::formatTerm(double coefficient, double exponent) {
    if (exponent == std::floor(exponent) && exponent < 0.0) {
        std::string term = formatNumber(coefficient) + "/x";
        if (exponent < -1.0) {
            term += "^" + formatNumber(-exponent);
        }
        return term;
    }

    if (exponent == 1.0) {
        return coefficient == 1.0 ? "x" : formatNumber(coefficient) + "x";
    }

    if (exponent == 0.0) {
        return formatNumber(coefficient);
    }

    std::string term = (coefficient == 1.0) ? "x" : formatNumber(coefficient) + "x";
    return term + "^" + formatNumber(exponent);
}

std::string Formatter::formatNumber(double value) {
    std::ostringstream oss;
    oss << std::setprecision(12) << value;
    return oss.str();
}

}  // namespace calc
