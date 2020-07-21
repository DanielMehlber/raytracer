#include "processing.h"

void process::start(){
    run();
    process_organizer::current_process_count--;
    delete this;
}

process::process(){
    while(process_organizer::current_process_count >= process_organizer::max_process_count);
    process_organizer::current_process_count++;
    std::thread th(&process::start, this);
    th.detach();
}

void process_organizer::wait() {
    while (!all_finished());
}

process::~process() {
}

size_t process_organizer::max_process_count = std::thread::hardware_concurrency();
size_t process_organizer::current_process_count = 0;