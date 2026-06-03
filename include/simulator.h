#pragma once

#include "algorithms.h"

struct Simulator {
    int actualTime;

    // Colas
    std::vector<Process*> initialList;
    std::vector<Process*> readyList;
    std::vector<Process*> blockedList;
    std::vector<Process*> terminatedList;
    std::vector<Process*> backupList;
    Process* runningProcess;

    Simulator();
    ~Simulator();

    void addProcess(int id, int arrivalTime, int timeCPU, int timeIO, int priority);
    void loadProcesses();
    //void loadRandomProcesses();
    void run();
    void runTick();
    //void updateMetrics();
    //void print();

    bool checkExit() const;
    void updateQueue(State state);
};