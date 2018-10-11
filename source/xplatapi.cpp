#include <chrono>
#include <thread>

void sleep(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}