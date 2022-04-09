cmake_minimum_required(VERSION 3.21)
project(thread_lab)

set(CMAKE_CXX_STANDARD 20)

add_executable(threads main.cpp semaphore.h)
add_executable(test semaphore_test.cpp semaphore.h)
add_executable(atomics atomic_count.cpp)
add_executable(prod-con sync_primitives/producer-consumer.cpp)
add_executable(read-write sync_primitives/readers-writers.cpp)
add_executable(sleeping_barber sync_primitives/sleeping_barber.cpp)
add_executable(philosophers sync_primitives/philosophers.cpp)
