#include "timer.h"

void timer_start(Timer* t) {
    clock_gettime(CLOCK_MONOTONIC, &t->start);
}

void timer_stop(Timer* t) {
    clock_gettime(CLOCK_MONOTONIC, &t->end);
}

double timer_elapsed_ms(Timer* t) {
    long seconds = t->end.tv_sec - t->start.tv_sec;
    long nanoseconds = t->end.tv_nsec - t->start.tv_nsec;
    return seconds * 1000.0 + nanoseconds / 1000000.0;
}
