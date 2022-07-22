#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

int main(int argc, char const *argv[]) {
    std::mutex mtx;
    std::condition_variable cv;
    std::string data;
    bool ready = false;
    bool processed = false;
    auto worker_thread = [&](){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&ready]{
            std::cout << "worker_thread wait\n";
            return ready;
        });

        std::cout << "Worker thread is processing data\n";
        data += " after processing";

        processed = true;
        std::cout << "Work thread signals data processing completed\n";

        lock.unlock();
        cv.notify_one();
    };

    std::thread worker(worker_thread);

    data = "Example data";
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();

    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&processed]{
            std::cout << "main_thread wait\n";
            return processed;
        });
    }

    std::cout << "Back int main(), data = " << data << "\n"; 
    worker.join();

    return 0;
}
