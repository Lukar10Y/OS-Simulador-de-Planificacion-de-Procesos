#pragma once

#include "algorithms.h"
#include <chrono>
#include <thread>

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
    int counterQuantum;
    int counterID;
    int idleTime;

    Simulator();
    ~Simulator();

    void addProcess(int arrivalTime, int timeCPU, int timeIO, int priority, int cycles = 1);
    void loadProcesses();
    //void loadRandomProcesses();
    void run(const float& time);
    void runTick();
    void runTick(const float& time);
    void getMetrics();
    void calcFinalMetrics();
    void getAverageMetrics();
    void print();
    void deleteInitialProcess(const int& id);

    bool checkExit() const;
    void updateQueue(State state);
    Process* doAlgorithm();
};