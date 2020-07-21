#include <thread>

class process_organizer {
public:
    static size_t max_process_count;
    static size_t current_process_count;
    inline static bool all_finished() { return current_process_count == 0; };
    static void wait();
};

class process {
protected:
    virtual void run() = 0;
    void start();
public:
    process();
    virtual ~process();
};