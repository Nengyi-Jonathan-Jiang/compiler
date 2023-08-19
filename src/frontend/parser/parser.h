#pragma once

#include <string>
#include <utility>
#include <vector>
#include <ranges>
#include <numeric>
#include <memory>
#include "../../util/util.h"
#include "../../common/symbol.h"
#include "common/syntax_tree.h"

class parser {
    syntax_tree::ParseTree<symbol> parse(std::vector<token> tokens);
    
};