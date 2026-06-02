#pragma once

#include "process.h"
#include <vector>
#include <string>

struct Simulator {
    int actualTime;
    std::vector<Process*> initialList;
    std::vector<Process*> readyList;
    std::vector<Process*> blockedList;
    std::vector<Process*> terminatedList;
    Process* runningProcess;

    Simulator();

    void addProcess(int id, int arrivalTime, int timeCPU, int timeIO, int priority);
    void loadProcesses();
    //void loadRandomProcesses();
    void run();
    void runTick();
    //void updateMetrics();
    //void print();

    bool checkExit() const;
};