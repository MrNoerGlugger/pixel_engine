#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <tuple>

class FunctionHolder {
public:
    // Template to store any callable with varying arguments
    template<typename Func, typename... Args>
    void store(Func&& func, Args&&... args) {
        // Create a bound function that captures the callable and its arguments
        auto boundFunc = [this, func = std::forward<Func>(func), argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable {
            callFunc(func, std::index_sequence_for<Args...>{}, argsTuple);
        };
        functions.push_back(boundFunc);
    }

    // Template to store any lambda expression
    template<typename Func>
    void storeLambda(Func&& func) {
        functions.push_back(func);
    }

    void callAll() const;

private:
    std::vector<std::function<void()>> functions; // Store callables with no arguments

    // Helper function to unpack and call the stored function with arguments
    template<typename Func, std::size_t... Is, typename Tuple>
    void callFunc(Func&& func, std::index_sequence<Is...>, Tuple&& args) {
        func(std::get<Is>(args)...); // Call the function with unpacked arguments
    }
};