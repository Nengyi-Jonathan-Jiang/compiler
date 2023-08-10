#pragma once

#include "syntax_tree/node.h"
#include "syntax_tree/leafNode.h"
#include "syntax_tree/internalNode.h"

namespace syntax_tree {
    template<typename T> struct AbstractSyntaxTree {
        ASTNode<T> root;

        AbstractSyntaxTree() = default;
        explicit AbstractSyntaxTree(ASTNode<T> root) : root(root) {}

        template<typename U> static ASTLeafNode<T> createLeafNode(T type, const U &value) {
            return std::make_shared<syntax_tree::_ASTLeafNode<T>>(type, value);
        }
        static ASTLeafNode<T> createLeafNode(T type) {
            return std::make_shared<syntax_tree::_ASTLeafNode<T>>(type);
        }
        static ASTInternalNode<T> createInternalNode(T type, const typename _ASTInternalNode<T>::children_t &children) {
            return std::make_shared<_ASTInternalNode<T>>(type, children);
        }
        static ASTLeafNode<T> castToLeafNode(ASTNode<T> n) {
            return std::dynamic_pointer_cast<_ASTLeafNode<T>>(n);
        }
        static ASTInternalNode<T> castToInternalNode(ASTNode<T> n) {
            return std::dynamic_pointer_cast<_ASTInternalNode<T>>(n);
        }
    };

    template<typename T> struct ParseTree {
        ASTNode<T> root;

        ParseTree() = default;
        explicit ParseTree(ParseTreeNode<T> root) : root(root) {}
    };
}