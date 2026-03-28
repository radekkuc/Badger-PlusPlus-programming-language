#pragma once
#include <type_traits>

template<typename T>
constexpr bool is_numeric = std::is_same_v<T,int> || std::is_same_v<T,float>;