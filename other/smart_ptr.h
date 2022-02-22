// 手写个智能指针(shared_ptr)类
#include <cstddef>
template <typename T>
class smart_ptr{
public:

    // 构造函数
    explicit smart_ptr(T *ptr = nullptr):
        real_ptr(ptr), linked_cnt(new size_t){
        if(real_ptr != nullptr)
            *linked_cnt = 1;
    }
    // 析构函数
    ~smart_ptr() {
        -- (*linked_cnt);
        if(*linked_cnt == 0) {
            delete real_ptr;
            delete linked_cnt;
            real_ptr = nullptr;
            linked_cnt = nullptr;
        }
    }
    // 拷贝构造函数
    smart_ptr(const smart_ptr& ptr) {
        linked_cnt = ptr.linked_cnt;
        real_ptr = ptr.real_ptr;
        ++(*linked_cnt);
    }
    // 拷贝赋值函数
    smart_ptr& operator=(const smart_ptr &rhs) {
        if(this != &rhs) {
            -- (*linked_cnt);
            if(*linked_cnt == 0) {
                delete real_ptr;
                delete linked_cnt;
            }
            real_ptr = rhs.real_ptr;
            linked_cnt = rhs.linked_cnt;
            ++ (*linked_cnt);
        }
        return *this;
    }
    // 移动构造函数
    smart_ptr(smart_ptr &&rhs): 
        real_ptr(rhs.real_ptr), linked_cnt(rhs.linked_cnt) {
        rhs.real_ptr = nullptr;
        rhs.linked_cnt = nullptr;
    }
    // 移动赋值运算符
    smart_ptr& operator=(smart_ptr &&rhs) {
        if(this != rhs) {
            -- (*linked_cnt);
            if(*linked_cnt == 0) {
                delete real_ptr;
                delete linked_cnt;
            }
            real_ptr = rhs.real_ptr;
            linked_cnt = rhs.linked_cnt;
            rhs.real_ptr = nullptr;
            rhs.linked_cnt = nullptr;
        }
        return *this;
    }
    // 解引用
    T& operator*() const {return *real_ptr;}
    // 箭头运算
    T& operator->() const {return real_ptr;}
    // 重载bool运算符
    operator bool() {return real_ptr == nullptr;}
    T* get() const {return real_ptr;}
    size_t use_count() const {return *linked_cnt;}
    bool unique() const {return *linked_cnt == 1;}
    void swap(smart_ptr& rhs) {swap(*this, rhs);}
private:
    T*      real_ptr;
    size_t* linked_cnt; // unsigned
};