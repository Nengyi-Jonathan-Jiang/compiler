#pragma once

#include <string>
#include <utility>
#include <vector>
#include <ranges>
#include <numeric>
#include <memory>
#include "../../util/util.h"
#include "../../common/symbol.h"

//const std::string empty_string = "";
//
//struct SymbolString {
//    std::vector<symbol> symbols;
//    size_t size;
//    std::string repr;
//
//    SymbolString(std::initializer_list<symbol> s)
//            : SymbolString(std::vector<symbol>(s)) {}
//
//    explicit SymbolString(const std::vector<symbol> &s)
//            : symbols(s), size(s.size()), repr(
//            s | std::ranges::accumulate<symbol, symbol>(
//                    bin_op(symbol, (!a ? "" : a->name + ' ') + b->name),
//                    nullptr
//            )
//    ) {}
//
//    const std::string &operator[](int i) const {
//        if (i < 0) {
//            i += static_cast<int>(size);
//        }
//        if (i >= size) {
//            return empty_string;
//        }
//        return symbols[i]->name;
//    }
//
//    auto &operator()(int i) const {
//        return operator[](i);
//    }
//
//    SymbolString operator()(int start, int end) const {
//        return SymbolString(std::views::iota(start, end)
//                            | std::views::transform([this](int i) -> std::string { return operator[](i); })
//                            | std::ranges::to<std::vector<std::string>>());
//    }
//
//    explicit operator std::string() const {
//        return repr;
//    }
//
//    explicit operator std::vector<std::string>() const {
//        return symbols;
//    }
//
//    auto operator<=>(const SymbolString &other) const {
//        return repr <=> other.repr;
//    }
//
//    [[nodiscard]] auto begin() const { return symbols.begin(); }
//
//    [[nodiscard]] auto end() const { return symbols.end(); }
//
//    SymbolString operator+(const SymbolString &other) const {
//        std::vector<std::string> result(size + other.size);
//        std::copy(begin(), end(), result.begin());
//        std::copy(
//                other.begin(),
//                other.end(),
//                result.begin() + static_cast<int>(size)
//        );
//        return SymbolString(result);
//    }
//
//    SymbolString operator+(const std::string &other) const {
//        return operator+(SymbolString{other});
//    }
//};
//
//struct ParseRule {
//    const std::string lhs;
//    const SymbolString rhs;
//
//    ParseRule(std::string lhs, SymbolString rhs)
//            : lhs(std::move(lhs)), rhs(std::move(rhs)) {}
//
//    [[nodiscard]] size_t size() const { return rhs.size; }
//
//    [[nodiscard]] bool empty() const { return rhs.size == 0; }
//
//    explicit operator std::string() {
//        return lhs + " :=" + static_cast<std::string>(rhs);
//    }
//
//    std::strong_ordering operator<=>(const ParseRule &other) {
//        std::strong_ordering o1 = lhs <=> other.lhs;
//        return o1 == std::strong_ordering::equal ? rhs <=> other.rhs : o1;
//    }
//};

class parser {

};