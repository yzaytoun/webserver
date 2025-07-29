/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-25
 * Last modified: 2024-12-25
 */

#include "Timer.hpp"

Timer::Timer() : _running(false){
};

Timer::~Timer() {
};

void Timer::Initialize() {
    memset(&_startTime, 0, sizeof(struct timeval));
    memset(&_endTime, 0, sizeof(struct timeval));
};

struct timeval Timer::GetStartTime() {
    return _startTime;
};

void Timer::SetStartTime() {
    _running = true;
    gettimeofday(&_startTime, NULL);
};

struct timeval Timer::GetEndTime() {
    return _endTime;
};

void Timer::SetEndTime() {
    _running = false;
    gettimeofday(&_endTime, NULL);
};

int64_t Timer::GetMilliSeconds() {
    return (CalcTotalUSec() / MILLISECONDS_CONVERSION);
};

int64_t Timer::GetMicroSeconds() {
    return (CalcTotalUSec());
};

int64_t Timer::GetSeconds() {
    return (CalcTotalUSec() / MICROSECONDS_CONVERSION);
};

int64_t Timer::GetCurrentTime() {
    struct timeval tmpTime;

    gettimeofday(&tmpTime, NULL);
    return ((tmpTime.tv_sec * MICROSECONDS_CONVERSION) + tmpTime.tv_usec);
};

int64_t Timer::CalcTotalUSec() {
    return (((_endTime.tv_sec - _startTime.tv_sec) * MICROSECONDS_CONVERSION) +
                                    (_endTime.tv_usec - _startTime.tv_usec));
};

int64_t Timer::TimeDiffFromCurrent() {
    struct timeval currTime;

    gettimeofday(&currTime, NULL);
    return (timvalToSeconds(currTime) - timvalToSeconds(_startTime));
};

int64_t timvalToMilliSeconds(struct timeval time) {
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}
int64_t timvalToMicroSeconds(struct timeval time) {
    return (time.tv_sec * 1000000) + time.tv_usec;
}
int64_t timvalToSeconds(struct timeval time) {
    return time.tv_sec + time.tv_usec / 1000000;
}

bool    Timer::isRunning()
{
    return (_running);
}
