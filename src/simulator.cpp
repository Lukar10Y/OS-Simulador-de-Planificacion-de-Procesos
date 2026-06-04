#include "simulator.h"

Simulator::Simulator() {
        actualTime = 0;
        runningProcess = nullptr;
        //FCFS, SJF, NPP, RAND, SRTF, PP, RR
        algorithm = RR;
        quantum = 2;
        counter = 0;
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
    // ID, Arrival Time, CPU Burst Time, I/O Burst Time, Priority
    addProcess(1, 0, 10, 3, 6);
    addProcess(2, 1, 2, 3, 2);
    addProcess(3, 2, 2, 3, 3);
    addProcess(4, 3, 2, 3, 5);
    addProcess(5, 4, 2, 3, 4);
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
                process->remainingTimeCPU = process->initialTimeCPU;
                readyList.push_back(process);
                std::cout << "      [Process Unblocked] ID: " << process->id << "\n";
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

Process* Simulator::doAlgorithm() {
    if(runningProcess == nullptr) {
        switch(algorithm) {
            case FCFS:
                return doFCFS(readyList);
                break;
            case SJF:
                return doSJF(readyList);
                break;
            case NPP:
                return doNPP(readyList);
                break;
            case RAND:
                return doRAND(readyList);
                break;
            default:                    
                break;
        }   
    }
    switch(algorithm) {
        case SRTF:
            return doSRTF(readyList, runningProcess);
            break;
        case PP:    
            return doPP(readyList, runningProcess);
            break;
        case RR:
            return doRR(readyList, runningProcess, counter, quantum);
        default:
            break;
    }
    return nullptr; 
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
    updateQueue(BLOCKED);
    if(runningProcess != nullptr) {
        std::cout << "  [Running Process] ID: " << runningProcess->id << "\n";
        --(runningProcess->remainingTimeCPU);
        if(runningProcess->remainingTimeCPU <= 0) {
            if(runningProcess->remainingTimeIO > 0) {
                runningProcess->state = BLOCKED;
                blockedList.push_back(runningProcess);
                std::cout << "      [Process Blocked] ID: " << runningProcess->id << "\n";
                runningProcess = nullptr;
            }
            else {
                runningProcess->state = TERMINATED;
                runningProcess->completionTime = actualTime;
                terminatedList.push_back(runningProcess);
                std::cout << "          [Process Terminated] ID: " << runningProcess->id << "\n";
                runningProcess = nullptr;
            }
        }
    }
    updateQueue(READY);
    Process* selected = doAlgorithm();
    if(selected != nullptr) {
        if(runningProcess != nullptr) {
            runningProcess->state = READY;
            readyList.push_back(runningProcess);
        }
        runningProcess = selected;
        runningProcess->state = RUNNING;
        readyList.erase(std::remove(readyList.begin(), readyList.end(), selected), readyList.end());
    }
    if(runningProcess != nullptr) {
        std::cout << "  [Process Running] ID: " << runningProcess->id << "\n";
    }
}