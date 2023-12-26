#include <queue>
#include <mutex>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {}

    void push(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        lock.unlock();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        T value = queue_.front();
        queue_.pop();
        lock.unlock();
        return value;
    }

    bool isEmpty()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        bool value = queue_.empty();
        lock.unlock();
        return value;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
};
