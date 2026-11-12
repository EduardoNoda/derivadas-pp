#include "../../include/factory/FunctionFactory.h"

#include "../../include/errors/Errors.h"
#include "../../include/math/LogarithmicFunction.h"
#include "../../include/math/PowerFunction.h"

namespace calc {

std::unique_ptr<MathFunction> FunctionFactory::create(const ParsedTerm& term) {
    switch (term.type) {
        case FunctionType::POWER:
            return std::make_unique<PowerFunction>(term.coefficient, term.exponent);

        case FunctionType::LOGARITHMIC:
            return std::make_unique<LogarithmicFunction>(term.coefficient);

        case FunctionType::UNKNOWN:
            throw UnknownFunctionError(term.functionName);
    }
    throw UnknownFunctionError("desconhecida");  // enum invalido: nunca deve acontecer
}

}  // namespace calc
