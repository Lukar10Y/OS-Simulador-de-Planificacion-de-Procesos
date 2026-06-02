#include "process.h"

Process::Process(int _id, int _arrivalTime, int _cpu, int _io, int _priority) {
        id = _id;
        arrivalTime = _arrivalTime;
        priority = _priority;
        initialTimeCPU = remainingTimeCPU = _cpu;
        initialTimeIO = remainingTimeIO = _io;
        state = READY;
        waitingTime = 0;
        blockedTime = 0;
        completionTime = 0;
        totalTime = 0;
}