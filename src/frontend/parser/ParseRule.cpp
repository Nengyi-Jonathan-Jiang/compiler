//
// Created by njona on 8/12/2023.
//

#include "ParseRule.h"

#include <utility>

int parseRule_::_id = 0;

parseRule_::parseRule_(symbol lhs, SymbolString rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)), id(++_id) {}

symbol parseRule_::getLhs() const {
    return lhs;
}

const SymbolString &parseRule_::getRhs() const {
    return rhs;
}

size_t parseRule_::getRhsSize() const {
    return rhs.size();
}

bool parseRule_::isEmpty() const {
    return getRhsSize() == 0;
}

std::strong_ordering parseRule_::operator<=>(const parseRule_ &other) const {
    return id <=> other.id;
}


