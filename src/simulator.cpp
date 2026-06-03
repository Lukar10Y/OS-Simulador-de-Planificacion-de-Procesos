#include "simulator.h"

Simulator::Simulator() {
        actualTime = 0;
        runningProcess = nullptr;
}

Simulator::~Simulator() {
    std::cout << "\n[Simulator] Limpiando memoria dinámica del Heap..." << std::endl;
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
    std::cout << "[Simulator] ¡Memoria liberada con éxito! Todos los bloques devueltos al S.O.\n" << std::endl;
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
    for(size_t i = 0; i < initialList.size(); ++i){
        if(initialList[i]->arrivalTime == actualTime){
            readyList.push_back(initialList[i]);
            initialList.erase(initialList.begin() + i);
            --i;
            readyList.back()->state = READY;
            readyList.back()->print();
        }
    }
}