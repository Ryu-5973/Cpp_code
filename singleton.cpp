// 单例模式C++实现
#include <iostream>
#include <atomic>
#include <mutex>

// 懒汉式
// 特点: 延迟加载
class Singleton1{
public:
    static Singleton1* get_instance();
private:
    Singleton1() = default;
    Singleton1(const Singleton1&) = default;

    static std::atomic<Singleton1*> p_instance;
    static std::mutex m_mutex;
};
std::atomic<Singleton1*> Singleton1::p_instance(nullptr);
std::mutex Singleton1::m_mutex;
Singleton1* Singleton1::get_instance() {
    Singleton1* tmp = p_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);//获取内存fence
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = p_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new Singleton1;
            std::atomic_thread_fence(std::memory_order_release);//释放内存fence
            p_instance.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}


// 饿汉式


int main() {
    Singleton1 *p1 = Singleton1::get_instance();
    Singleton1 *p2 = Singleton1::get_instance();
    Singleton1 *p3 = p2->get_instance();
    std::cout << (long)p1 << std::endl;
    std::cout << (long)p2 << std::endl;
    std::cout << (long)p3 << std::endl;
    return 0;
}