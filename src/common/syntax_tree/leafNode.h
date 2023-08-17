#pragma once

#include "node.h"
#include "../../util/util.h"
#include <any>
#include <vector>
#include <utility>
#include <memory>

namespace syntax_tree {
    template<typename T>
    struct _ASTLeafNode : public syntax_tree::_ASTNode<T> {
        explicit _ASTLeafNode(T type) : syntax_tree::__baseTreeNode<T>(type),
                                        syntax_tree::_ASTNode<T>(type), value() {}

        template<typename U>
        _ASTLeafNode(T type, U value) : syntax_tree::__baseTreeNode<T>(type),
                                        syntax_tree::_ASTNode<T>(type),
                                        value(value) {}

        template<typename U>
        const U &get_value() const {
            return any_cast<const U &>(value);
        }

        template<typename U>
        [[nodiscard]] bool holds_type() const {
            return value.type() == typeid(U);
        }

        bool isLeaf() final {
            return true;
        }

    private:
        const std::any value;
    };

    template<typename T> using ASTLeafNode = std::shared_ptr<_ASTLeafNode<T>>;

    template<typename T>
    struct _ParseTreeLeafNode : public syntax_tree::_ParseTreeNode<T> {
        explicit _ParseTreeLeafNode(T type) : syntax_tree::__baseTreeNode<T>(type),
                                              syntax_tree::_ParseTreeNode<T>(type) {}

        template<typename U>
        _ParseTreeLeafNode(T type, const U& value) : syntax_tree::__baseTreeNode<T>(type),
                                                     syntax_tree::_ParseTreeNode<T>(type, value) {}

        bool isLeaf() final {
            return true;
        }
    };

    template<typename T> using ASTLeafNode = std::shared_ptr<_ASTLeafNode<T>>;
}