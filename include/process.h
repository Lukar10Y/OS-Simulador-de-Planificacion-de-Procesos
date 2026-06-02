#pragma once

#include <iostream>

enum State {
    READY = 0,
    RUNNING = 1,
    BLOCKED = 2,
    TERMINATED = 3
};

struct Process {
    int id;                 
    int arrivalTime;     
    int priority;          

    // CPU Burst Time
    int initialTimeCPU;
    int remainingTimeCPU;

    // I/O Burst Time
    int initialTimeIO;
    int remainingTimeIO;

    State state; 

    // Métricas
    int waitingTime;      
    int blockedTime;
    int completionTime;
    int totalTime;

    Process(int _id, int _arrivalTime, int _cpu, int _io, int _priority);

    void print() const {
        std::cout << " [Process Created] ID: " << id 
                << " | Arrival: " << arrivalTime 
                << " | CPU Burst: " << initialTimeCPU 
                << " | I/O Burst: " << initialTimeIO 
                << " | Priority: " << priority << "\n";
    }
};