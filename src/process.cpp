#include "../include/process.h"

Process::Process(int _id, int _arrivalTime, int _CPU, int _IO, int _priority, int _cycles) {
    id = _id;
    arrivalTime = _arrivalTime;
    priority = _priority;
    initialTimeCPU = remainingTimeCPU = _CPU;
    initialTimeIO = remainingTimeIO = _IO;
    state = IDLE;
    waitingTime = 0;
    completionTime = 0;
    turnAroundTime = 0;
    initialCycles = remainingCycles = _cycles;
}

std::string Process::getState() const {
    switch(state){
        case IDLE: return "IDLE";
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case BLOCKED: return "BLOCKED";
        case TERMINATED: return "TERMINATED";
        default: return "UNKNOWN";
    }
}

void Process::print() const {
    std::cout << " [Process] ID: " << id 
                << " | Arrival: " << arrivalTime 
                << " | CPU Burst: " << remainingTimeCPU 
                << " | I/O Burst: " << remainingTimeIO 
                << " | Priority: " << priority 
                << " | State: " << getState() 
                << " | Cycles: " << initialCycles << "\n";
}