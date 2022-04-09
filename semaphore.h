//
// Created by katari on 4/9/22.
//

#ifndef THREAD_LAB_SEMAPHORE_H
#define THREAD_LAB_SEMAPHORE_H
#include <mutex>
#include <condition_variable>

template<int Count = 1>
class Semaphore {
public:
    Semaphore() = default;

    Semaphore(int counter) : internal_counter_(std::min(Count, counter)) {}

    const int &count() const { return internal_counter_; }

    void release() {
        std::unique_lock<std::mutex> u_lock(mtx_);
        if (internal_counter_ < Count) {
            ++internal_counter_;
        }
        free_.notify_one();
    }

    void acquire() {
        std::unique_lock<std::mutex> u_lock(mtx_);
        while (internal_counter_ == 0)
            free_.wait(u_lock);
        --internal_counter_;
    }

    bool try_acquire() {
        std::unique_lock<std::mutex> u_lock(mtx_);
        if (internal_counter_ > 0) {
            --internal_counter_;
            return true;
        } else {
            return false;
        }
    }

private:
    std::mutex mtx_;
    std::condition_variable free_;
    int internal_counter_ = Count;

};
#endif //THREAD_LAB_SEMAPHORE_H
