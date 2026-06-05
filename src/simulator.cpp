#include "simulator.h"

Simulator::Simulator() {
        actualTime = 0;
        runningProcess = nullptr;
        //FCFS, SJF, NPP, RAND, SRTF, PP, RR
        algorithm = RR;
        quantum = 2;
        counterQuantum = 0;
        counterID = 1;
}

Simulator::~Simulator() {
    std::cout << "\n[Simulator] Limpiando memoria dinamica del Heap..." << std::endl;
    for (Process* process : backupList) {
        if (process != nullptr) {
            delete process;
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

void Simulator::addProcess(int arrivalTime, int timeCPU, int timeIO, int priority, int cycles) {
    initialList.push_back(new Process(counterID, arrivalTime, timeCPU, timeIO, priority, cycles));
    backupList.push_back(initialList.back());
    std::cout << "[Process Added]\n";
    initialList.back()->print();
    ++counterID;
}

void Simulator::loadProcesses() {
    // ID, Arrival Time, CPU Burst Time, I/O Burst Time, Priority, cycles = 1
    addProcess(0, 5, 3, 6, 2);
    addProcess(1, 2, 3, 2, 2);
    addProcess(2, 2, 3, 3, 2);
    addProcess(3, 2, 3, 5);
    addProcess(4, 2, 3, 4);
}

bool Simulator::checkExit() const {
    return initialList.empty() && readyList.empty() && blockedList.empty() && runningProcess == nullptr;
}

void Simulator::updateQueue(State state) {
    if(state == BLOCKED) {
        for(auto it = blockedList.begin(); it != blockedList.end(); ) {
            Process* process = *it;
            process->tickOnIOTime();
            if(process->remainingTimeIO <= 0) {
                process->state = READY;
                process->remainingTimeCPU = process->initialTimeCPU;
                if(process->remainingCycles > 1) {
                    process->tickOnCycles();
                }
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
            return doRR(readyList, runningProcess, counterQuantum, quantum);
        default:
            break;
    }
    return nullptr; 
}

void Simulator::run(const float& time) {
    std::cout << "      [RUNNING]\n";
    while(!checkExit())
    {
        runTick();
        std::chrono::duration<float> tick(time);
        std::this_thread::sleep_for(tick);
    }
    std::cout << "      [SIMULATION ENDED]\n";
    print();
    getMetrics();
    calcFinalMetrics();
    getAverageMetrics();
}

void Simulator::runTick() {
    std::cout << "[Tick] Time: " << actualTime << "\n";
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
    for(Process* process : readyList) {
        ++(process->waitingTime);
    }
    updateQueue(BLOCKED);
    if(runningProcess != nullptr) {
        std::cout << "  [Running Process] ID: " << runningProcess->id << "\n";
        runningProcess->tickOnCPUTime();
        if(runningProcess->remainingTimeCPU <= 0) {
            if(runningProcess->remainingTimeIO > 0 && runningProcess->remainingCycles > 0) {
                runningProcess->state = BLOCKED;
                blockedList.push_back(runningProcess);
                std::cout << "      [Process Blocked] ID: " << runningProcess->id << "\n";
                runningProcess = nullptr;
            }
            else {
                runningProcess->state = TERMINATED;
                runningProcess->tickOnCompletionTime(actualTime);
                terminatedList.push_back(runningProcess);
                std::cout << "          [Process Terminated] ID: " << runningProcess->id << "\n";
                runningProcess = nullptr;
            }
        }
    }
    else {
        ++idleTime;
    }
    ++actualTime;
}

void Simulator::runTick(const float& time) {
    std::cout << "[Tick] Time: " << actualTime << "\n";
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
    for(Process* process : readyList) {
        ++(process->waitingTime);
    }
    updateQueue(BLOCKED);
    if(runningProcess != nullptr) {
        std::cout << "  [Running Process] ID: " << runningProcess->id << "\n";
        runningProcess->tickOnCPUTime();
        if(runningProcess->remainingTimeCPU <= 0) {
            if(runningProcess->remainingTimeIO > 0 && runningProcess->remainingCycles > 0) {
                runningProcess->state = BLOCKED;
                blockedList.push_back(runningProcess);
                std::cout << "      [Process Blocked] ID: " << runningProcess->id << "\n";
                runningProcess = nullptr;
            }
            else {
                runningProcess->state = TERMINATED;
                runningProcess->tickOnCompletionTime(actualTime);
                terminatedList.push_back(runningProcess);
                std::cout << "          [Process Terminated] ID: " << runningProcess->id << "\n";
                runningProcess = nullptr;
            }
        }
    }
    else {
        ++idleTime;
    }
    ++actualTime;
    std::chrono::duration<float> tick(time);
    std::this_thread::sleep_for(tick);
}

void Simulator::print() {
    std::cout << "\n[Simulator State at Time " << actualTime << "]\n";
    std::cout << "  Running Process: " << (runningProcess ? std::to_string(runningProcess->id) : "None") << "\n";
    std::cout << "\n  Ready List: \n";
    for (const auto& process : readyList) {
        process->print();
    }
    std::cout << "\n  Blocked List: \n";
    for (const auto& process : blockedList) {
        process->print();
    }
    std::cout << "\n  Terminated List: \n";
    for (const auto& process : terminatedList) {
        process->print();
    }
}

void Simulator::calcFinalMetrics() {
    std::cout << "\n[Metrics]\n";
    for (const auto& process : terminatedList) {
        std::cout << "  Process ID: " << process->id 
                  << " | Waiting Time: " << process->completionTime - process->arrivalTime - process->initialTimeCPU*(process->initialCycles+1) - process->initialTimeIO*process->initialCycles
                  << " | Turnaround Time: " << process->completionTime - process->arrivalTime 
                  << " | Completion Time: " << process->completionTime 
                  << " | Block Time: " << process->initialTimeIO * (process->initialCycles) 
                  << " | Execution Time: " << process->initialTimeCPU * (process->initialCycles + 1) << "\n";
    }
}

void Simulator::getMetrics() {
        std::cout << "\n[Metrics]\n";
        for (const auto& process : terminatedList) {
            std::cout << "  Process ID: " << process->id 
                    << " | Waiting Time: " << process->waitingTime
                    << " | Turnaround Time: " << process->turnAroundTime 
                    << " | Completion Time: " << process->completionTime 
                    << " | Block Time: " << process->blockTime 
                    << " | Execution Time: " << process->executionTime << "\n";
        }
}

void Simulator::getAverageMetrics() {
    int totalProcesses = terminatedList.size();
    if (totalProcesses == 0) {
        std::cout << "\n[Average Metrics] No processes were terminated.\n";
        return;
    }

    double totalWaitingTime = 0;
    double totalTurnAroundTime = 0;
    double totalBlockTime = 0;
    double totalExecutionTime = 0;

    for (const auto& process : terminatedList) {
        totalWaitingTime += process->waitingTime;
        totalTurnAroundTime += process->turnAroundTime;
        totalBlockTime += process->blockTime;
        totalExecutionTime += process->executionTime;
    }

    std::cout << "\n[Average Metrics]\n";
    std::cout << "  Total Processes: " << totalProcesses << "\n";
    std::cout << "  CPU percentage used: " << ((actualTime - idleTime) / static_cast<double>(actualTime)) * 100 << "%\n";
    std::cout << "  Average Waiting Time: " << totalWaitingTime / totalProcesses << "\n";
    std::cout << "  Average Turnaround Time: " << totalTurnAroundTime / totalProcesses << "\n";
    std::cout << "  Average Block Time: " << totalBlockTime / totalProcesses << "\n";
    std::cout << "  Average Execution Time: " << totalExecutionTime / totalProcesses << "\n";
}

void Simulator::deleteInitialProcess(const int& id)
{
    Process* process = nullptr;
    for (Process* p : backupList) {
        if (p->id == id) {
            process = p;
            break;
        }
    }
    if (process != nullptr) {
        initialList.erase(
            std::remove(initialList.begin(), initialList.end(), process),
            initialList.end()
        );
        backupList.erase(
            std::remove(backupList.begin(), backupList.end(), process),
            backupList.end()
        );
        delete process;
    }
}