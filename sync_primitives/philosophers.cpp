//
// Created by katari on 4/9/22.
//
#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

using namespace std::chrono_literals;

void philosopher(int i, std::mutex &vector_lock, std::mutex &fork1_lock, std::mutex &fork2_lock, bool &end) {
    while (!end) {
        std::this_thread::sleep_for(10ms);
        std::lock_guard<std::mutex> take_table(vector_lock);
        std::cout << "Philosopher " << i << " takes table\n";
        {
            std::lock_guard<std::mutex> left(fork1_lock);
            std::cout << "Philosopher " << i << " takes fork " << i << "\n";
        }
        {
            std::lock_guard<std::mutex> left(fork2_lock);
            std::cout << "Philosopher " << i << " takes fork " << i + 1 << "\n";
        }
    }
}

int main() {
    int phils_count = 5;
    std::vector<std::mutex> forks(5);
    std::mutex table;
    std::vector<std::jthread> phils(phils_count);
    bool end = false;
    for (int i = 0; i < phils_count; ++i) {
        phils[i] = std::jthread(philosopher, i, std::ref(table), std::ref(forks[i]),
                                std::ref(forks[(i + 1) % phils_count]), std::ref(end));
    }
    std::this_thread::sleep_for(1000ms);
    end = true;
}
