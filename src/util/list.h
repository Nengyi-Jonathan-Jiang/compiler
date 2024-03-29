#pragma once

#include <ranges>
#include <iterator>
#include <algorithm>
#include <string>
#include <set>
#include <cstddef>
#include <cassert>

template <class T> class list {
    T* contents;

public:
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using difference_type = std::size_t;
    using size_type = std::size_t;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;

        explicit iterator(T* ptr) : ptr(ptr) {}
        iterator(const iterator& it) : ptr(it.ptr) {}

        iterator& operator=(const iterator&) =default;
        bool operator==(const iterator& it) { return ptr == it.ptr; }
        bool operator!=(const iterator& it) { return ptr != it.ptr; }
        std::strong_ordering operator<=>(const iterator& it) {
            return ptr <=> it.ptr;
        }

        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) { iterator res = *this; ++*this; return res; }
        iterator& operator--() { --ptr; return *this; }
        iterator operator--(int) { iterator res = *this; --*this; return res; }

        iterator& operator+=(size_type x){ ptr += x; return *this; }
        iterator operator+(size_type x) const { iterator res = *this; return res += x; }
        iterator& operator-=(size_type x) { ptr -= x; return *this; }
        iterator operator-(size_type x) const { iterator res = *this; return res -= x; }

        difference_type operator-(iterator it) const { return ptr - it.ptr; }

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }
        reference operator[](size_type x) const { return ptr[x]; }

    private:
        pointer ptr;
    };

    class const_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;

        explicit const_iterator (T* ptr) : ptr(ptr) {}
        const_iterator (const const_iterator& it) : ptr(it.ptr) {}
        const_iterator (const iterator& it) : ptr(it.ptr) {} // NOLINT(google-explicit-constructor)

        const_iterator& operator=(const const_iterator&) =default;
        std::strong_ordering operator<=>(const const_iterator& it) {
            return ptr <=> it.ptr;
        }

        const_iterator& operator++() { ++ptr; return *this; }
        const_iterator operator++(int) { const_iterator res = *this; ++*this; return res; }
        const_iterator& operator--() { --ptr; return *this; }
        const_iterator operator--(int) { const_iterator res = *this; --*this; return res; }

        const_iterator& operator+=(size_type x) { ptr += x; return *this; }
        const_iterator operator+(size_type x) const { iterator res = *this; return res += x; }
        const_iterator& operator-=(size_type x) { ptr -= x; return *this; }
        const_iterator operator-(size_type x) const { iterator res = *this; return res -= x; }

        difference_type operator-(const_iterator it) const { return ptr - it.ptr; }

        reference operator*() const { return *ptr; }
        const_pointer operator->() const { return ptr; }
        reference operator[](size_type x) const { return ptr[x]; }

    private:
        const_pointer ptr;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    list() : N(0), ptr(nullptr) {}
    explicit list(int size) : N(size), ptr(new T[size]) {}

    template<class ...Args> list(int size, Args&&... args) : N(size), ptr(new T[size]){
        for(T& x : *this) x = T(std::forward<Args>(args)...);
    }

    list(std::initializer_list<T> elements) : N(elements.size()), ptr(new T[elements.size()]){
        iterator thisIt = begin();
        iterator thisEnd = end();
        auto elementsIt = elements.begin();
        auto elementsEnd = elements.end();

        while(thisIt != thisEnd){
            *thisIt++ = *elementsIt++;
        }
    }

    list(const list& other) : N(other.N), ptr(new T[other.N]) { memcpy(ptr, other.ptr, N); }
    ~list(){ delete ptr; }


    auto operator<=>(const list& l) const { return std::lexicographical_compare_three_way(cbegin(), cend(), l.cbegin(), l.cend()); }

    iterator begin() { return iterator(ptr); }
    const_iterator begin() const { return const_iterator(ptr); }
    const_iterator cbegin() const { return const_iterator(ptr); }
    iterator end() { return iterator(ptr + N); }
    const_iterator end() const { return const_iterator(ptr + N); }
    const_iterator cend() const { return const_iterator(ptr + N); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

    reference front() { return ptr[0]; }
    const_reference front() const { return ptr[0]; }
    reference back() { return ptr[N - 1]; }
    const_reference back() const { return ptr[N - 1]; }
    reference operator[](size_type x) { return ptr[x]; }
    const_reference operator[](size_type x) const { return ptr[x]; }
    reference at(size_type x) { return ptr[x]; }
    const_reference at(size_type x) const { return ptr[x]; }

    [[nodiscard]] size_type size() const { return N; }
    [[nodiscard]] size_type max_size() const { return N; }
    [[nodiscard]] bool empty() const { return N == 0; }

private:
    int N;
    pointer ptr;
};