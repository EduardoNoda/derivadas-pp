#include "../../include/application/DerivativeService.h"

#include <memory>
#include <vector>

#include "../../include/errors/Errors.h"
#include "../../include/math/DerivativeResult.h"
#include "../../include/math/MathFunction.h"

namespace calc {

CalculationResult DerivativeService::calculate(const std::string& expression) const {
    try {
        const auto tokens = lexer_.tokenize(expression);

        const auto terms = parser_.parse(tokens);

        std::vector<DerivativeResult> results;
        results.reserve(terms.size());
        for (const ParsedTerm& term : terms) {
            std::unique_ptr<MathFunction> function = FunctionFactory::create(term);
            results.push_back(function->derive());
        }
        return CalculationResult{true, formatter_.format(results)};

    } catch (const UnknownFunctionError& e) {
        return CalculationResult{false, e.what()};
    } catch (const SyntaxError& e) {
        return CalculationResult{false, e.what()};
    } catch (const std::exception& e) {
        return CalculationResult{false,
                                 std::string("Erro inesperado: ") + e.what()};
    }
}

}  // namespace calc
