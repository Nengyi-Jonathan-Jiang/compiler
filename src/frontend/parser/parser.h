#pragma once

#include <string>
#include <utility>
#include <vector>
#include <ranges>
#include <numeric>
#include <memory>
#include "../../util/util.h"
#include "../../common/Symbol.h"
#include "common/syntax_tree.h"

class parser {
public:
    virtual syntax_tree::ParseTree<symbol> parse(std::vector<token> tokens) = 0;
};