#include "simulator.h"

Simulator::Simulator() {
        actualTime = 0;
        runningProcess = nullptr;
}

Simulator::~Simulator() {
    std::cout << "\n[Simulator] Limpiando memoria dinamica del Heap..." << std::endl;
    for (Process* proc : backupList) {
        if (proc != nullptr) {
            delete proc;
        }
    }
    initialList.clear();
    readyList.clear();
    blockedList.clear();
    terminatedList.clear();
    backupList.clear();
    runningProcess = nullptr;
    std::cout << "[Simulator] Memoria liberada con exito. Todos los bloques devueltos al S.O.\n" << std::endl;
}

void Simulator::addProcess(int id, int arrivalTime, int timeCPU, int timeIO, int priority) {
    initialList.push_back(new Process(id, arrivalTime, timeCPU, timeIO, priority));
    backupList.push_back(initialList.back());
    std::cout << "[Process Added]\n";
    initialList.back()->print();
}

void Simulator::loadProcesses() {
    addProcess(1, 0, 5, 3, 1);
    addProcess(2, 1, 5, 3, 2);
    addProcess(3, 2, 5, 3, 3);
    addProcess(4, 3, 5, 3, 4);
    addProcess(5, 4, 5, 3, 5);
}

bool Simulator::checkExit() const {
    return initialList.empty() && readyList.empty() && blockedList.empty() && runningProcess == nullptr;
}

void Simulator::updateQueue(State state) {
    if(state == BLOCKED) {
        for(auto it = blockedList.begin(); it != blockedList.end(); ) {
            Process* process = *it;
            --(process->remainingTimeIO);
            if(process->remainingTimeIO <= 0) {
                process->state = READY;
                readyList.push_back(process);
                std::cout << "  [Process Unblocked] ID: " << process->id << "\n";
                it = blockedList.erase(it);
            } else {
                ++it;
            }
        }
    }
    else if(state == READY) {
        for(auto it = initialList.begin(); it != initialList.end(); ) {
            Process* process = *it;
            if(process->arrivalTime == actualTime) {
                process->state = READY;
                readyList.push_back(process);
                std::cout << "  [Process Arrived] ID: " << process->id << "\n";
                it = initialList.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void Simulator::run() {
    loadProcesses();
    std::cout << "      [RUNNING]\n";
    while(!checkExit())
    {
        runTick();
        //updateMetrics();
        //print();
        ++actualTime;
    }
}

void Simulator::runTick() {
    std::cout << "[Tick] Time: " << actualTime << "\n";
    if(runningProcess != nullptr){
        std::cout << "  [Running Process] ID: " << runningProcess->id << "\n";
        --(runningProcess->remainingTimeCPU);
        if(runningProcess->remainingTimeCPU <= 0) {
            runningProcess->state = TERMINATED;
            runningProcess->completionTime = actualTime;
            terminatedList.push_back(runningProcess);
            std::cout << "      [Process Terminated] ID: " << runningProcess->id << "\n";
            runningProcess = nullptr;
        }
    }
    updateQueue(BLOCKED);
    updateQueue(READY);
    if(runningProcess == nullptr) {
        runningProcess = FCFS(readyList);
        if(runningProcess != nullptr){
            std::cout << "  [Process Running] ID: " << runningProcess->id << "\n";
        }
    }
}