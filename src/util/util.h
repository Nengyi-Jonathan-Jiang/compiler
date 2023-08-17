#pragma once

#include <ranges>
#include <iterator>
#include <algorithm>
#include <string>
#include <set>
#include <cstddef>
#include <iterator>
#include <cassert>

#define bin_op(input, x) [](const input& a, const input& b)->typeof(x){return x;}

#define panic {throw "Unreachable code";}
#define noop {}

template<typename T, std::ranges::range U>
std::set<T> make_set(const U &container) {
    return {container.begin(), container.end()};
}

namespace detail {
    using namespace std;
    using namespace std::ranges;

    template<typename C>
    struct to {
    };

    template<typename C, range R>
    requires convertible_to<range_value_t<R>, typename C::value_type>
    auto operator|(R &&r, to<C>) {
        return C{r.begin(), r.end()};
    }

    template<typename T, typename U>
    struct accumulate {
        const U (*func)(const U &, const T &);

        const U initial;

        accumulate(U (*func)(const U &, const T &), U initial) : func(func), initial(initial) {}
    };

    template<typename T, typename U, range R>
    U operator|(R &&r, accumulate<T, U> accumulate) {
        U res = accumulate.initial;
        for (auto it = r.begin(); it != r.end(); it++) {
            res = accumulate.func(res, *it);
        }
        return res;
    }

}

namespace std::ranges { // NOLINT(cert-dcl58-cpp)
    template<std::ranges::range C>
    requires (!std::ranges::view<C>)
    auto to() {
        return detail::to < C > {};
    }

    template<typename T, typename U>
    auto accumulate(U (*func)(const U &, const T &), U initial) {
        return detail::accumulate <T, U> {func, initial};
    }
}