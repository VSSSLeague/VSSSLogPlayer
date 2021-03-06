#include "timer.h"

Timer::Timer() {
    // Updating time1 and time2 with actual time
    clock_gettime(CLOCK_REALTIME, &_time1);
    clock_gettime(CLOCK_REALTIME, &_time2);
}

void Timer::start() {
    // Updating time1 with last time
    clock_gettime(CLOCK_REALTIME, &_time1);
}

void Timer::stop() {
    // Updating time2 with last time
    clock_gettime(CLOCK_REALTIME, &_time2);
}

double Timer::getSeconds() {
    return (getNanoSeconds() / 1E9);
}

double Timer::getMiliSeconds() {
    return (getNanoSeconds() / 1E6);
}

double Timer::getMicroSeconds() {
    return (getNanoSeconds() / 1E3);
}

double Timer::getNanoSeconds() {
    // Get time2 - time1
    return ((_time2.tv_sec * 1E9) + _time2.tv_nsec) - ((_time1.tv_sec * 1E9) + _time1.tv_nsec);
}

QString Timer::getActualTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    char str[100];
    sprintf(str, "%04d-%02d-%02d|%02d:%02d:%02d", 1900+ltm->tm_year, 1+ltm->tm_mon, ltm->tm_mday,
                                                  ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

    QString actualTime(str);

    return actualTime;
}

qint64 Timer::systemTime() {
    return QDateTime::currentMSecsSinceEpoch();
}
