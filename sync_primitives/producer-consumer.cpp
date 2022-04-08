//
// Created by katari on 3/26/22.
//

#include <iostream>
#include <thread>
#include "../semaphore.h"
#include <mutex>
#include <stack>
#include <vector>


using namespace std::chrono_literals;
constexpr int max_semaphore_value = 20;


void
producer(std::mutex &mtx, Semaphore<max_semaphore_value> &full, Semaphore<max_semaphore_value> &empty, std::stack<int> &
st) {
    for (int i = 0; i < std::numeric_limits<int>::max(); ++i) {
        full.acquire();
        mtx.lock();
        st.push(i);
        std::cout << i << " produced\ncount_empty = " << empty.count() << "\ncount_full = " << full.count() << "\n";
        mtx.unlock();
        empty.release();
    }
}

void
consumer(std::mutex &mtx, Semaphore<max_semaphore_value> &full, Semaphore<max_semaphore_value> &empty, std::stack<int> &
st) {
    std::cout << "reader";
    for (int i = 0; i < std::numeric_limits<int>::max(); ++i) {
        empty.acquire();
        mtx.lock();
        std::cout << st.top() << " consumed\ncount_empty = " << empty.count() << "\ncount_full = " << full.count()
                  << "\n";
        st.pop();

        mtx.unlock();
        full.release();
    }
}

int main() {
    std::mutex mtx;
    Semaphore<max_semaphore_value> full, empty(0);
    std::stack<int> st;
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        if (i % 2) {
            threads.emplace_back(&producer, std::ref(mtx), std::ref(full), std::ref(empty), std::ref(st));
        } else {
            threads.emplace_back(&consumer, std::ref(mtx), std::ref(full), std::ref(empty), std::ref(st));

        }
    }
    std::this_thread::sleep_for(100ms);
    for (int i = 0; i < threads.size(); ++i) {
        std::cout << "terminating thread" << std::endl;
        threads[i].std::thread::~thread();
    }

}

