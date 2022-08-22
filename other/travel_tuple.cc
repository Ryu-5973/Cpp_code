/*
 * @Author: Ryu-59073
 * @Date: 2022-07-26 08:30:49
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-08-08 02:44:13
 * @FilePath: /Cpp_code/other/travel_tuple.cc
 * @Description: tuple 遍历
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <tuple>

using namespace std;

// C++11 模板类遍历
template<typename Tuple, size_t N>
struct traverse{
    static void print(Tuple& t) {
        traverse<Tuple, N-1>::print(t);
        cout << get<N-1>(t);
    }
};
template<typename Tuple>
struct traverse<Tuple, 1>{
    static void print(Tuple& t) {
        cout << get<0>(t);
    }
};
template<typename ...Args>
void travel_tuple_cpp11(tuple<Args...>& t) {
    traverse<decltype(t), sizeof...(Args)>::print(t);
}

// C++14 index_sequence
template<typename ...Args>
void travel_tuple_cpp14(tuple<Args...>& t) {
    const auto& traverse = []<typename Tuple, size_t... Index>
    (Tuple& t, index_sequence<Index...>) {
        (cout << ... << get<Index>(t));
    };
    traverse(t, index_sequence_for<Args...>{});
    // cout << endl;
}

// C++17 apply
template<typename Tuple>
void travel_tuple_cpp17(Tuple& t) {
    std::apply([](auto &&... args){
        (cout << ... << args);
    }, std::move(t));
}

template<typename ...Args>
void prt(Args&& ...args) {
    ((cout << args << " "), ...) << endl;
}

template<typename ...Args>
std::ostream& operator<<(ostream& os, std::tuple<Args...> &t) {
    travel_tuple_cpp11(t);
    return std::cout;
}

int main(int argc, char const *argv[]) {
    auto t = make_tuple(1, 2.0, '3', "4", "五");
    // travel_tuple_cpp11(t);
    // travel_tuple_cpp14(t);
    // travel_tuple_cpp17(t);
    std::cout << t << std::endl;
    prt(true, 2, 3.0, '4', "五");
    prt(3);
    return 0;
}
