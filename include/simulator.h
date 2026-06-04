#pragma once

#include "algorithms.h"

enum AlgorithmType {
    FCFS = 0,
    SJF = 1,
    NPP = 2,
    RAND = 3,
    SRTF = 4,
    PP = 5,
    RR = 6
};

struct Simulator {
    int actualTime;

    // Colas
    std::vector<Process*> initialList;
    std::vector<Process*> readyList;
    std::vector<Process*> blockedList;
    std::vector<Process*> terminatedList;
    std::vector<Process*> backupList;
    Process* runningProcess;
    AlgorithmType algorithm;
    int quantum;
    int counter;

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
    Process* doAlgorithm();
};