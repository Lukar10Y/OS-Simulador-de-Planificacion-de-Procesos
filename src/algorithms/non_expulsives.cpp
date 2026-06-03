#include "../../include/algorithms.h"

Process* doFCFS(std::vector<Process*>& readyList) {
    if (readyList.empty()) {
        return nullptr;
    }
    Process* process = readyList.front();
    process->state = RUNNING;
    readyList.erase(readyList.begin());
    return process;
}

Process* doSJF(std::vector<Process*>& readyList) {
    if (readyList.empty()) {
        return nullptr;
    }
    auto it = std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
        return a->remainingTimeCPU < b->remainingTimeCPU;
    });
    Process* process = *it;
    process->state = RUNNING;
    readyList.erase(it);
    return process;
}

Process* doNPP(std::vector<Process*>& readyList) {
    if (readyList.empty()) {
        return nullptr;
    }
    auto it = std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
        return a->priority < b->priority;
    });
    Process* process = *it;
    process->state = RUNNING;
    readyList.erase(it);
    return process;
}