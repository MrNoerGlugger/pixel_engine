#include "FunctionHolder.hpp"

void FunctionHolder::callAll() const {
    for (const auto& func : functions) {
        func(); // Call each stored function
    }
}