#pragma once

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <queue>

class ThreadPool {
    public:
        ThreadPool(size_t num_threads);
        ~ThreadPool();
        void AddTask(std::function<void()> task);
        void Stop();
        bool IsStopped();
    private:
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> tasks;
        std::mutex mutex;
        std::condition_variable cv;
        bool stop;
};
