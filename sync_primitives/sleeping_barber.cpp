//
// Created by katari on 4/2/22.
//
#include "../semaphore.h"
#include <queue>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void barber(std::mutex &seats, Semaphore<15> &customer, Semaphore<1> &barber, std::queue<int> &clients, bool &end,
            int &free_seats) {
    while (true) {
        int customer_id;
        std::cout << __LINE__ << " " << clients.size() << "\n";
        if (end && clients.empty()) {
            std::cout << __LINE__ << " " << "\n";
            break;
        }
        customer.acquire();
        {
            std::cout << __LINE__ << " " << "\n";
            std::lock_guard<std::mutex> lg(seats);
            std::cout << __LINE__ << " " << "\n";
            ++free_seats;
            customer_id = clients.front();
            clients.pop();

        }
        barber.release();
        std::cout << "cutting_hair_of_customer " << customer_id << "\n";
    }
    std::cout << "Barber sem: " << barber.count() << " " << customer.count() << " " << free_seats << "\n";
    std::cout << "Barber goes to sleep. Mafia awakes.\n";
}

void client(std::mutex &mtx, Semaphore<15> &customer, Semaphore<1> &barber, std::queue<int> &clients, bool &end,
            int &free_seats, int &id) {
    std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
    while (!end) {
        std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
        mtx.lock();
        std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
        if (free_seats > 0 && !end) {
            std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
            --free_seats;
            std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
            customer.release();
            std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
            clients.push(id);
            ++id;
            std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
            mtx.unlock();
            std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
            barber.acquire();
            std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
            std::cout << "customer " << id << "\n";
        } else {
            std::cout << __LINE__ << " " << std::this_thread::get_id() << "\n";
            mtx.unlock();
        }
    }
    while (!clients.empty()) {
        customer.release();
        barber.acquire();
    }

    std::cout << "\nSemaphore: " << barber.count() << " " << customer.count() << " " << free_seats << "\n";
    std::cout << "Client was finished!\n";
}

int main() {
    Semaphore<1> barber_s(0);
    Semaphore<15> customer(0);
    std::mutex mtx;
    int free_seats = 15;
    std::queue<int> clients;
    bool end = false;
    int id = 0;
    std::thread barber0(&barber, std::ref(mtx), std::ref(customer), std::ref(barber_s), std::ref(clients),
                        std::ref(end), std::ref(free_seats));
    std::thread customer0(&client, std::ref(mtx), std::ref(customer), std::ref(barber_s), std::ref(clients),
                          std::ref(end), std::ref(free_seats), std::ref(id));
    std::thread customer1(&client, std::ref(mtx), std::ref(customer), std::ref(barber_s), std::ref(clients),
                          std::ref(end), std::ref(free_seats), std::ref(id));
    std::this_thread::sleep_for(500ms);
    end = true;
    barber0.join();
    customer0.join();
    customer1.join();
    return 0;
}
