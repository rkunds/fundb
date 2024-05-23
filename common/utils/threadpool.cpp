#include "threadpool.h"

ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; i++) {
        threads.push_back(std::thread([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    cv.wait(lock, [this] { return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) {
                        return;
                    }
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    Stop();
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }

    cv.notify_all();

    for (auto& thread : threads) {
        thread.join();
    }

    threads.clear();
}

void ThreadPool::AddTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.push(task);
    }

    cv.notify_one();
}