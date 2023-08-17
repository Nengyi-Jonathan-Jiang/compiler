#pragma once

namespace util {
    template<typename T>
    class const_ptr {
    public:
        using value_t = T;
        using pointer_t = const value_t *;
        using reference_t = const value_t &;

        const_ptr() : ptr(nullptr) {};

        const_ptr(pointer_t ptr) : ptr(ptr) {};

        const_ptr &operator=(pointer_t p) {
            this->ptr = p;
        }

        pointer_t operator->() const {
            return ptr;
        }

        reference_t operator*() const {
            return *ptr;
        }

    private:
        pointer_t ptr;
    };
}