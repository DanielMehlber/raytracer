#include "timing.h"

Clock::Clock() {
    start();
}

double Clock::start() {
    last_start = std::chrono::system_clock::now();
    return last_start.time_since_epoch().count();
}

double Clock::stop() {
    last_stop = std::chrono::system_clock::now();
    auto delta = last_stop.time_since_epoch().count() - last_start.time_since_epoch().count();
    return delta;
}