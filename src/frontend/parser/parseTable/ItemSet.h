#pragma once

#include <set>
#include "frontend/parser/parseRule.h"
#include "frontend/parser/symbolSet.h"

class Item {

public:
    parseRule rule;
    int pos;
    symbolSet lookahead;

    explicit Item(const parseRule &rule, int pos, symbolSet lookahead) : rule(rule), pos(pos), lookahead(lookahead) {}

    std::weak_ordering operator<=> (const Item& other) const {
        return std::tie(pos, rule, lookahead) <=> std::tie(other.pos, other.rule, lookahead);
    }

    bool isFinished() {
        return pos >= rule->getRhsSize();
    }

    symbol next() {
        return isFinished() ? nullptr : rule->getRhs()[pos];
    }
};

class ItemSet {
    std::set<Item> items;

    ItemSet() = default;
    ItemSet(Item item) : items{item} {}

    std::weak_ordering operator<=>(const ItemSet& other) const {
        return items <=> other.items;
    }
};