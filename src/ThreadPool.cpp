#include "ThreadPool.h"

ThreadPool::ThreadPool(int num_threads) : toStop(false)
{
    for (int i = 0; i < num_threads; i++)
    {
        threads.push_back(std::thread([this]() {
            while (true) {
                std::function<void()> task;
                std::unique_lock<std::mutex> lock(tasks_mutex);

                cv.wait(lock, [this]() { return toStop || !tasks.empty(); });
                
                if (toStop && tasks.empty()) {
                    return;
                }
                task = std::move(tasks.front());
                tasks.pop();

                task();
            } 
        }));
    }
}

ThreadPool::~ThreadPool()
{
    toStop = true;
    cv.notify_all();
    for (std::thread &t : threads) 
    {
        if (t.joinable()) 
            t.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) 
{
    std::unique_lock<std::mutex> lock(tasks_mutex);
    tasks.push(std::move(task));
    cv.notify_one();
}