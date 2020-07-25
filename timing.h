#include <chrono>

/**
 * @brief Measures time. Used for render time.
 */
class Clock {
public:
    std::chrono::system_clock::time_point last_start;
    std::chrono::system_clock::time_point last_stop;
    /**
     * @brief Construct a new Clock object and starts time measurement.
     */
    Clock();
    /**
     * @brief Starts time measurement.
     * @return double time of starting measurement.
     */
    double start();
    /**
     * @brief Stops time measurement.
     * @return double measured time in nano-seconds.
     */
    double stop();
};