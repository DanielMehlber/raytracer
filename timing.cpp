//        __            _      __               __    ____             
//   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
//  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
// / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
// \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
//  https://github.com/danielmehlber                                     

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