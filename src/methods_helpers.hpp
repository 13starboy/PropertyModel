#include <any>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace NSMethodsHelpers {

template <typename T>
struct type_identity {
    using type = T;
};

using AnyFunction = std::function<std::any(const std::vector<std::any>&)>;

namespace detail {

template <typename R, typename F, typename... Args, std::size_t... I>
std::any invokeHelper(F func, const std::vector<std::any>& vec, std::index_sequence<I...>) {
    if constexpr (std::is_same_v<R, void>) {
        std::invoke(func, std::any_cast<std::remove_cvref_t<Args>>(vec[I])...);
        return std::any{};
    } else {
        R ret = std::invoke(func, std::any_cast<std::remove_cvref_t<Args>>(vec[I])...);
        return std::any{std::move(ret)};
    }
}

template <typename R, typename F, typename... Args>
AnyFunction makeAnyFunctionImpl(F func, type_identity<R(Args...)>) {
    return [func](const std::vector<std::any>& vec) -> std::any {
        if (vec.size() != sizeof...(Args)) {
            throw std::runtime_error(
                "Wrong number of arguments passed to AnyFunction. Expected " + std::to_string(sizeof...(Args)) +
                ", got " + std::to_string(vec.size())
            );
        }
        return invokeHelper<R, F, Args...>(func, vec, std::index_sequence_for<Args...>{});
    };
}

}  // namespace detail

template <typename F>
AnyFunction makeAnyFunction(F func) {
    using Fun = std::remove_reference_t<F>;

    static_assert(
        std::is_pointer_v<Fun> && std::is_function_v<std::remove_pointer_t<Fun>>,
        "makeAnyFunction only accepts a free-function pointer, e.g. &MyFunc"
    );

    using FptrT = std::remove_pointer_t<Fun>;

    return detail::makeAnyFunctionImpl(func, type_identity<FptrT>{});
}
}  // namespace NSMethodsHelpers
