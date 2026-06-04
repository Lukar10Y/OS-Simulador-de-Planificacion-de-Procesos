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

    int initialCycles;
    int remainingCycles;

    // Métricas
    int waitingTime;      
    int completionTime;
    int turnAroundTime;

    Process(int _id, int _arrivalTime, int _CPU, int _IO, int _priority, int _cycles = 1);

    std::string getState() const;
    void print() const;
};