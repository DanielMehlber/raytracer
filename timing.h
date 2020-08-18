//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

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