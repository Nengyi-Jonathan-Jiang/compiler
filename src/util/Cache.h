#pragma once

#include <map>

template<typename T, typename U>
struct Cache {
private:
    std::map<T, U> map;
public:
    Cache() = default;

    template<typename V>
    requires std::invocable<V, const T &>
    const U &computeIfAbsent(T key, V func);

    void store(T key, U value);

    const U &operator[](const T &key) const;
};

template<typename T, typename U>
template<typename V>
requires std::invocable<V, const T &>
const U &Cache<T, U>::computeIfAbsent(T key, V func) {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    }
    return map.emplace(key, func(key)).first->second;
}


template<typename T, typename U>
const U &Cache<T, U>::operator[](const T &key) const {
    return map.at(key);
}

template<typename T, typename U>
void Cache<T, U>::store(T key, U value) {
    map.emplace(std::move(key), std::move(value));
}