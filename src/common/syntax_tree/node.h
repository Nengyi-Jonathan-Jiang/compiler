#pragma once

#include "../../util.h"
#include <any>
#include <vector>
#include <utility>
#include <memory>

namespace syntax_tree {
    template<typename T> struct __baseTreeNode {
        const T type;
        explicit __baseTreeNode(T type) : type(type) {}

        virtual bool isLeaf() = 0;
    };

    template<typename T> struct _ASTNode : public virtual __baseTreeNode<T> {
        explicit _ASTNode(T type) : __baseTreeNode<T>(type) {}
    };

    template<typename T> using ASTNode = std::shared_ptr<_ASTNode<T>>;

    template<typename T> struct _ParseTreeNode: public virtual __baseTreeNode<T> {
        const std::any value;

        explicit _ParseTreeNode(T type) : __baseTreeNode<T>(type), value{} {}

        template<typename U>
        explicit _ParseTreeNode(T type, const U& value) : __baseTreeNode<T>(type), value(value) {}

        template<typename U>
        [[nodiscard]] const U& get_value() const {
            return std::any_cast<const U&>(value);
        }

        template<typename U>
        [[nodiscard]] bool holds_type() const {
            return value.type() == typeid(U);
        }
    };

    template<typename T> using ParseTreeNode = std::shared_ptr<_ParseTreeNode<T>>;
}