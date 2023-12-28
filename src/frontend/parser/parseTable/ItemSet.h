#pragma once

#include <set>
#include <utility>
#include "frontend/parser/ParseRule.h"
#include "frontend/parser/symbolSet.h"

class Item {

public:
    parseRule rule;
    int pos;
    symbolSet lookahead;

    Item(parseRule rule, int pos, symbolSet lookahead) : rule(std::move(rule)), pos(pos), lookahead(std::move(lookahead)) {}

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

    std::weak_ordering operator<=>(const ItemSet& other) const {
        return items <=> other.items;
    }

public:
    ItemSet(Item item) : items{std::move(item)} {}
};