#include <thread>

/**
 * @brief Manages amount of running processes.
 */
class process_organizer {
public:
    /**
     * @brief Max amount of processes allowed to run in parallel.
     */ 
    static size_t max_process_count;
    /**
     * @brief Current amount of running processes (always < max amount).
     */ 
    static size_t current_process_count;
    /**
     * @brief Are all processes finished?
     */ 
    inline static bool all_finished() { return current_process_count == 0; };
    /**
     * @brief Waits until all processes are finished (= until porcess count equals 0).
     */
    static void wait();
};

/**
 * @brief virtual process managing a thread.
 */
class process {
protected:
    /**
     * @brief What the process executes when activated.
     */
    virtual void run() = 0;
    /**
     * @brief starts the process. 
     */
    void start();
public:
    /**
     * @brief Creates and starts new process inside thread.
     */ 
    process();
    virtual ~process();
};