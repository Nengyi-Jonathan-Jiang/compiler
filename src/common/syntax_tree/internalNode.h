#pragma once

#include "leafNode.h"
#include "node.h"
#include "../../util.h"
#include <any>
#include <vector>
#include <utility>
#include <memory>

namespace syntax_tree {
    template<typename T, typename U>
    struct __baseInternalNode : public virtual syntax_tree::__baseTreeNode<T> {
        typedef std::vector<U> children_t;
        children_t children;

        explicit __baseInternalNode(T type, children_t children) : syntax_tree::__baseTreeNode<T>(type),
                                                                   children(children) {}

        void add_child(const U& child){
            children.push_back(child);
        }

        children_t::iterator begin() { return children.begin(); }

        children_t::iterator end() { return children.end(); }

        U operator[](size_t index) {
            return children[index];
        }

        bool isLeaf() final {
            return false;
        }
    };

    template<typename T>
    struct _ASTInternalNode : public __baseInternalNode<T, ASTNode<T>>, public _ASTNode<T> {
        typedef __baseInternalNode<T, ASTNode<T>>::children_t children_t;

        _ASTInternalNode(T type, const children_t &children) : __baseTreeNode<T>(type),
                                                               __baseInternalNode<T, ASTNode<T>>(type, children),
                                                               _ASTNode<T>(type) {}
    };

    template<typename T> using ASTInternalNode = std::shared_ptr<_ASTInternalNode<T>>;

    template<typename T>
    struct _ParseTreeInternalNode : public __baseInternalNode<T, ParseTreeNode<T>>, public _ParseTreeNode<T> {
        typedef __baseInternalNode<T, ParseTreeNode<T>>::children_t children_t;

        _ParseTreeInternalNode(T type, const children_t &children) : __baseTreeNode<T>(type),
                                                                     __baseInternalNode<T, ParseTreeNode<T>>(children),
                                                                     _ParseTreeNode<T>(type) {}

        template<typename U>
        _ParseTreeInternalNode(T type, const U &value, const children_t &children) : __baseTreeNode<T>(type),
                                                                                     __baseInternalNode<T, ParseTreeNode<T>>(children),
                                                                                     _ParseTreeNode<T>(type, value) {}
    };
    template<typename T> using ParseTreeInternalNode = std::shared_ptr<_ParseTreeInternalNode<T>>;
}