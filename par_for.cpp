

#include "par_for.h"


/**
 * get the number of threads that the system can handle
 * @return unsigned int
 */
unsigned int getThreadNumber() {
    const static unsigned nb_threads_hint = std::thread::hardware_concurrency();
    const static unsigned nb_threads = (nb_threads_hint == 0u ? 8u : nb_threads_hint);

    return nb_threads;
}



void Timer::start( const std::chrono::milliseconds& interval, const std::function<void(void)>& fn ) {
    _running = true;

    _th = std::thread([=](){
                            while ( _running ) {
                                std::this_thread::sleep_for(interval);
                                fn();
                            }
                        });
}


void Timer::stop( ) {
    _running = false;
    _th.join();
}

