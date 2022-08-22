#include <iostream>

template<typename V, typename U>
struct is_same{
    operator bool() {
        return false;
    }
};

template<>
struct is_same<int8_t, int8_t>{
    operator bool() {
        return true;
    }
};

template<typename T>
struct is_same<T, int8_t>{
    operator bool() {
        return false;
    }
};

template<typename T>
struct is_same<T, double>{
    operator bool() {
        return false;
    }
};

template<>
struct is_same<double, double>{
    operator bool() {
        return true;
    }
};

// template<typename V, typename U>
// struct type_traits{
//     static bool value = false;
// };

template<typename T>
void func(T val) {
    if(is_same<T, int8_t>()) {
        std::cout << "int8_t" << std::endl;
    }else if(is_same<T, double>()) {
        std::cout << "double" << std::endl;
    }else {
        std::cout << "unkonwn" << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    double val;
    std::cin >> val;
    func(val);
    return 0;
}


