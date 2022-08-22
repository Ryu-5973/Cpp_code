/*
 * @Author: Ryu-59073
 * @Date: 2022-08-01 02:14:26
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-08-01 08:03:40
 * @FilePath: /Cpp_code/other/tuple_foward.cc
 * @Description: std::tuple 的完美转发
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <tuple>
#include <string>
#include <functional>

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& theTuple) {
    std::apply (
        [&os](Ts const&... tupleArgs) {
            os << '[';
            std::size_t n{0};
            ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
            os << ']';
        }, theTuple
    );
    return os;
}


template<typename... Args>
std::istream& operator>>(std::istream& is, 
            std::tuple<Args...> &&t) {
    std::apply ([&is](Args &... tupleArgs) {
        (std::cin >> ... >> tupleArgs);
    }, t);
    return is;
}

// class Serializer{
//     template<typename... Args>
//     Serializer &operator >> (std::tuple<Args...> &&t){
//         /**
//          * @brief 实际的反序列化函数，利用折叠表达式展开参数包
//          */
//         std::apply([this](Args &... args) {
//             ((*this) >> ... >> args);
//         }, );
//         return *this;
//     }
// };
template<typename... Ts>
void read(std::tuple<Ts...> & theTuple) {
    std::apply (
        [](Ts &... tupleArgs) {
             (std::cin >> ... >> tupleArgs);
        }, theTuple
    );
}


// template<typename ...Args>
// void WriteTuple(std::tuple<Args...> &t) {
//     std::apply([](auto &&... args){
//         (std::cin >> ... >> args);
//     },t);
// }

int main(int argc, char const *argv[]) {
    // char *str = "四";
    auto t = std::make_tuple(false, 1, '2', std::string("3"));
    // auto t = std::make_tuple(false, 1, 4, 2, 1);
    std::cout << t << std::endl;
    // read(t);
    std::cout << t << std::endl;
    // ReadTuple(t);
    // WriteTuple(t);
    // ReadTuple(t);
    return 0;
}


