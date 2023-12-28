#pragma once

#include <memory>
#include "SymbolString.h"

class parseRule_ {
    static int _id;

    symbol lhs;
    SymbolString rhs;
    int id;
public:
    parseRule_(symbol lhs, SymbolString rhs);

    [[nodiscard]] symbol getLhs() const;
    [[nodiscard]] const SymbolString& getRhs() const;

    [[nodiscard]] size_t getRhsSize() const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] std::strong_ordering operator<=>(const parseRule_& other) const;

};

using parseRule = std::shared_ptr<parseRule_>;