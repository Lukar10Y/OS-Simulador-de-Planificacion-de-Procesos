#pragma once

#include <iostream>
#include <string>

enum State {
    IDLE = 0,
    READY = 1,
    RUNNING = 2,
    BLOCKED = 3,
    TERMINATED = 4
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

    Process(int _id, int _arrivalTime, int _CPU, int _IO, int _priority);

    std::string getState() const;
    void print() const;
};