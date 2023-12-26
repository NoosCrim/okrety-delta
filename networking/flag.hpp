#include <mutex>
#include <condition_variable>

//kod wzorowany kodem z stackoverflow  https://stackoverflow.com/questions/57280865/best-way-to-wait-for-flag-in-thread
class Flag
{
public:
    Flag() : flag_{false} {}



    void set()
    {
        std::lock_guard g(mutex_);
        flag_ = true;
        cond_var_.notify_all();
    }

    void clear()
    {
        std::lock_guard g(mutex_);
        flag_ = false;
    }

    void wait()
    {
        std::unique_lock lock(mutex_);
        cond_var_.wait(lock, [this]() { return flag_; });
    }

private:
    bool flag_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};
