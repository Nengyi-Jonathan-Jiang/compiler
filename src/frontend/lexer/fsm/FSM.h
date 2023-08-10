#pragma once

#include <map>
#include <set>
#include <memory>
#include "../../../util.h"

class FSM {
public:
    using state_t = uint32_t;
    using entry_t = std::map<char, std::set<state_t>>;
    using table_t = std::map<state_t, entry_t>;

    constexpr static state_t ERROR_STATE = 0xfffffffful;

    const static std::set<char> all_chars, word_chars, letters, digits;
};