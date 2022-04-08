//
// Created by katari on 3/26/22.
//

#include <iostream>
#include <thread>
#include "../semaphore.h"
#include <mutex>
#include <stack>
#include <shared_mutex>
#include <vector>

using namespace std::chrono_literals;
constexpr int max_semaphore_value = 20;

void writer(std::shared_mutex &mtx, std::string& data, bool& end) {
    for (int i = 0; ; ++i) {
        std::unique_lock<std::shared_mutex> ex_lock(mtx);
        data += std::to_string(i) + " ";
        std::cout << "written: " << data << "\n";
        if (end) {
            std::cout << "No fanfics any more!\n";
            return;
        }
    }

}

void
reader(std::shared_mutex &mtx, const std::string& data, bool& end) {
    for (int i = 0;; ++i) {
        std::shared_lock<std::shared_mutex> sh_lock(mtx);
        std::cout << "read: " << data << "\n";
        std::this_thread::sleep_for(7ms);
        if (end) {
            std::cout << "There are better fanfics in another fandom!\n";
            return;
        }
    }
}

int main() {
    std::shared_mutex mtx;
    Semaphore<max_semaphore_value> full, empty(0);
    std::string s;
    bool end = false;
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        if (i % 3) {
            threads.emplace_back(&reader, std::ref(mtx), std::ref(s), std::ref(end));
        } else {
            threads.emplace_back(&writer, std::ref(mtx), std::ref(s), std::ref(end));

        }
    }
    std::this_thread::sleep_for(1000ms);
    end = true;

    for (auto& thread : threads) {
        thread.join();
    }
}

