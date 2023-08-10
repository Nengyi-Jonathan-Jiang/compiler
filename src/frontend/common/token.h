#pragma once

#include "../../util.h"
#include "../../common/symbol.h"
#include <memory>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>
#include <string>
#include <map>

const std::string STRING_NULL;
using id_t = size_t;

struct token {
    symbol type;
    std::string value;
    size_t startIndex, endIndex;

    token(symbol type, std::string value, size_t startIndex, size_t endIndex);

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] bool is_end_token() const;

    [[nodiscard]] operator bool() const;

    static token create_end_token(size_t position);
};