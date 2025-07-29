/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-25
 * Last modified: 2024-12-25
 */

#ifndef TIMER_HPP
# define TIMER_HPP

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#define MILLISECONDS_CONVERSION 1000
#define MICROSECONDS_CONVERSION 1000000

class Timer {
public:
    Timer();
    ~Timer();

    void Initialize();
    bool isRunning();

    struct timeval GetStartTime();
    void SetStartTime();

    struct timeval GetEndTime();
    void SetEndTime();

    int64_t GetMilliSeconds();
    int64_t GetMicroSeconds();
    int64_t GetSeconds();

    int64_t GetCurrentTime();
    int64_t TimeDiffFromCurrent();

private:
    int64_t CalcTotalUSec();
    bool    _running;

    struct timeval  _startTime;
    struct timeval  _endTime;
};

// Auxiliar fuction time
int64_t timvalToMilliSeconds(struct timeval time);
int64_t timvalToMicroSeconds(struct timeval time);
int64_t timvalToSeconds(struct timeval time);

#endif // TIMER_H
