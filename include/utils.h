#pragma once
#include <iostream>
#include <type_traits>

template<typename T>
constexpr bool is_numeric = std::is_same_v<T,int> || std::is_same_v<T,float>;

template<typename... Args>
void log(Args... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}