#pragma once

#include <set>
#include <vector>

/// Computes the compute_closure of s by repeatedly applying the transformation s -> {...s, ...f(x) for x in s}
/// f is guaranteed to be called exactly once on every element that ends up in s
/// Set has_manual_check to true if f checks for membership in s
template<typename T>
void compute_closure(std::set<T> &s, auto f, bool has_manual_check=false) {
    std::set<T> edge {s.begin(), s.end()};
    while (!edge.empty()) {
        const auto it = edge.begin();
        const T element = *it;
        edge.erase(it);

        for (const T &x: f(element)) {
            if (has_manual_check || s.insert(x).second) {
                edge.insert(x);
            }
        }
    }
}