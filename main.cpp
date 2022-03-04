#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <chrono>

using namespace std::chrono_literals;
double avg(const std::vector<int>& v) {
    int sum = 0;
    for (int i : v) {
        std::cout << "i from avg " << i << "\n";
        sum += i;
        std::this_thread::sleep_for(12ms);
    }
    return static_cast<double>(sum) / v.size();
}

std::pair<int, int> min_max(const std::vector<int>& v) {
    int min_element = std::numeric_limits<int>::max(), max_element = std::numeric_limits<int>::min();
    for (int i : v) {
        std::cout << "i from minmax: " << i << "\n";
        min_element = std::min(min_element, i);
        std::this_thread::sleep_for(7ms);
        max_element = std::max(max_element, i);
        std::this_thread::sleep_for(7ms);
    }
    return {min_element, max_element};
}

int main() {
    std::cout << "enter vector elements. Enter any non-int character as the end of vector\n";
    std::vector<int> test{std::istream_iterator<int>{std::cin}, std::istream_iterator<int>{}}; //{1, -1, 2, -10};

    std::future<double> avg_future = std::async(&avg, test);
    std::packaged_task<std::pair<int, int>(const std::vector<int>&)> pac_task(min_max);
    std::future<std::pair<int, int>> min_max_ftr = pac_task.get_future();
    std::thread thr(std::move(pac_task), test);
    thr.join();
    int average = avg_future.get();
    std::pair<int, int> mm_pair = min_max_ftr.get();
    for (int& i : test) {
        if (i == mm_pair.first || i == mm_pair.second) {
            i = average;
        }
    }
    for (int i : test) {
        std::cout << i << " ";
    }
    return 0;
}
