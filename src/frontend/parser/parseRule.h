#pragma once

#include <memory>
#include "symbolString.h"

class parseRule_ {
    static int _id;

    symbol lhs;
    symbolString rhs;
    int id;
public:
    parseRule_(symbol lhs, symbolString rhs);

    [[nodiscard]] symbol getLhs() const;
    [[nodiscard]] const symbolString& getRhs() const;

    [[nodiscard]] size_t getRhsSize() const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] std::strong_ordering operator<=>(const parseRule_& other) const;

};

using parseRule = std::shared_ptr<parseRule_>;