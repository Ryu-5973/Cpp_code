// 测试各种情况的sizeof

#include <iostream>

using namespace std;

class A{};    // 空类
class B{    // 只含有一个虚函数的类
    virtual void Func(){}
};
class C{    // 只含有构造函数和析构函数
    C() = default;
    ~C() = default;
};
class D{    // 只含有一个静态成员变量
    static int val;
};
class E{    // 只含有一个成员变量
    int val;
};
class F{    // 只含有一个普通函数
    int Func(){}
};

int getSize(int a[]){
    return sizeof(a);
}

int main() {
    // 对各种类的sizeof
    cout << "sizeof(A) = " << sizeof(A) << endl;    // 空类： 空类实例化得到一个独一无二的的地址，所以为1
    cout << "sizeof(B) = " << sizeof(B) << endl;    // 只含有一个虚函数：生成一个虚函数表含一个虚函数指针(8byte)
    cout << "sizeof(C) = " << sizeof(C) << endl;    // 只含有构造和析构函数
    cout << "sizeof(D) = " << sizeof(D) << endl;    // 只含有静态成员变量
    cout << "sizeof(E) = " << sizeof(E) << endl;    // 含有一个成员变量int(4byte)
    cout << "sizeof(F) = " << sizeof(F) << endl;    // 含有一个普通函数

    // 对数组的sizeof
    int arr[5];
    int *pa = arr;

    cout << "sizeof(arr) = " << sizeof(arr) << endl;    // 整个数组的大小
    cout << "sizeof(pa) = " << sizeof(pa) << endl;      // 指针(int*)的大小
    cout << "sizeof(getSize(arr)) = " << getSize(arr) << endl;  // 函数调用有一个数组类型向指针类型的转换


    return 0;
}