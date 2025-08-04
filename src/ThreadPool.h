#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::atomic<bool> toStop;
    std::mutex tasks_mutex;
    std::condition_variable cv;

public:
    ThreadPool(int num_threads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);
};