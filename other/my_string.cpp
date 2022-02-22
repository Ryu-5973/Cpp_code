#ifndef MY_STRING_H
#define MY_STRING

#include <string.h>
#include <iostream>
#include <vector>

class my_string{
public:
    my_string(const char* _str = nullptr){
        str = new char[strlen(_str) + 1];
        strcpy(str, _str);
    }
    my_string(const my_string &rhs) {
        str = new char[strlen(rhs.str) + 1];
        strcpy(str, rhs.str);
    }
    my_string(my_string &&rhs)noexcept: str(rhs.str){
        rhs.str = nullptr;
    }
    my_string& operator=(const my_string &rhs) {
        if(this != &rhs) {
            delete[] str;
            str = new char[strlen(rhs.str) + 1];
            strcpy(str, rhs.str);
        }
        return *this;
    }
    my_string& operator=(my_string &&rhs) {
        if(this != &rhs) {
            delete[] str;
            str = rhs.str;
            rhs.str = nullptr;
        }
        return *this;
    }
    ~my_string() {
        delete[] str;
        str = nullptr;
    }
    friend my_string operator+(my_string &rhs, const my_string &lhs) {
        int len_rhs = strlen(rhs.str), len_lhs = strlen(lhs.str);
        char *tmp = rhs.str;
        rhs.str = new char[len_lhs + len_rhs + 1];
        strcpy(rhs.str, tmp);
        strcat(rhs.str, lhs.str);
        delete[] tmp;
        return rhs;
    }
    friend std::ostream& operator<<(std::ostream &os, const my_string &rhs) {
        os << rhs.str;
        return os;
    }
private:
    char* str;
};

using std::cout;
using std::endl;

int main() {
    typedef my_string string;
    string a = "aaaa", b = "bbbb";
    a = "ccc";
    cout << a + b << endl;;
    return 0;
}

#endif