#include "../../include/algorithms.h"

Process* doFCFS(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    Process* process = readyList.front();
    process->state = RUNNING;
    readyList.erase(readyList.begin());
    return process;
}

Process* doSJF(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    auto it = std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
        return a->remainingTimeCPU < b->remainingTimeCPU;
    });
    Process* process = *it;
    process->state = RUNNING;
    readyList.erase(it);
    return process;
}

Process* doNPP(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    auto it = std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
        return a->priority < b->priority;
    });
    Process* process = *it;
    process->state = RUNNING;
    readyList.erase(it);
    return process;
}

Process* doRAND(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, readyList.size() - 1);
    int index = dis(gen);

    Process* process = readyList[index];
    readyList.erase(readyList.begin() + index);
    return process;
}