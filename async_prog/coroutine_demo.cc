#include <iostream>
#include <coroutine>
#include <functional>
#include <thread>
#include <future>


#include <unistd.h>
/**
 * @brief promise规范的类Task
 * 
 */
template<class T>
struct Task {
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    struct promise_type {
        Task get_return_object() {
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() {
            return {};
        }
        std::suspend_always final_suspend() noexcept {
            return {};
        }
        void return_value(T val) {
            m_val = val;
        }
        std::suspend_always yield_value(T val) {
            m_val = val;
            return {};
        }
        void unhandled_exception() {
            std::terminate();
        }
        T m_val;
    };
    Task() : m_handle(nullptr) {}
    Task(handle h): m_handle(h){}
    Task(const Task& rhs) = delete;
    Task(Task&& rhs) noexcept
            : m_handle(rhs.m_handle) {
        rhs.m_handle = nullptr;
    }

    Task& operator=(const Task& rhs) = delete;
    Task& operator=(Task&& rhs) noexcept {
        if (std::addressof(rhs) != this)
        {
            if (m_handle)
            {
                m_handle.destroy();
            }

            m_handle = rhs.m_handle;
            rhs.m_handle = nullptr;
        }

        return *this;
    }
    T get() {
        return m_handle.promise().m_val;
    }
    void resume() {
        m_handle.resume();
    }
    bool done() {
        return !m_handle || m_handle.done();
    }
    void destroy() {
        m_handle.destroy();
        m_handle = nullptr;
    }

    ~Task() {
        if (m_handle) {
            m_handle.destroy();
        }
    }
private:
    handle m_handle = nullptr;
};

int64_t getCnt() {
    static int64_t cnt = 1;
    return cnt ++;
}

struct Func2{
    Func2(int64_t _init = 0): init(_init), result(0){
    }
    bool await_ready() const {
        return false;
    }
    int64_t await_resume() {
        return result;
    }
    void await_suspend(std::coroutine_handle<> handle) {
        std::thread t([handle, this](){
            this->result = getCnt() + this->init;
            // handle.resume();
            handle();
        });
        t.detach();
    } 
    int64_t init;
    int64_t result;
};
// enum {
//     TEST
// };

Task<int64_t> accmulate(std::function<void(int64_t)> f) {
    std::cout << 1 << std::endl;
    int64_t val = co_await Func2();
    co_yield 312;
    val = co_await Func2(val);
    val = co_await Func2(val);  
    f(val);
    co_return 21;
}

int main(int argc, char const *argv[]) {
    // std::promise<int64_t> promise;
    // auto future = promise.get_future();
    // std::thread t([](std::promise<int64_t>& pms){
    //     std::promise<int64_t> promise;
    //     auto future = promise.get_future();
    //     std::thread t([](std::promise<int64_t>& pms){
    //         std::promise<int64_t> promise;
    //         auto future = promise.get_future();
    //         std::thread t([](std::promise<int64_t>& pms){
    //             pms.set_value(getCnt());
    //         }, std::ref(promise));
    //         t.detach();
    //         pms.set_value(future.get() + getCnt());
    //     }, std::ref(promise));
    //     t.detach();
    //     pms.set_value(future.get() + getCnt());
    // }, std::ref(promise));
    // t.detach();
    // std::cout << future.get() << std::endl;
    auto handle = accmulate([](int64_t val){
        std::cout << val << std::endl;
    });
    while(1) {
        getchar();
        handle.resume();
        std::cout << "main get: " << handle.get() << std::endl;
    }
    // getchar();
    return 0;
}