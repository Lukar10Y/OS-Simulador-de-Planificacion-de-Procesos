#include "../../include/algorithms.h"

Process* doFCFS(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    return *readyList.begin();
}

Process* doSJF(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    return *std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
        return a->remainingTimeCPU < b->remainingTimeCPU;
    });
}

Process* doNPP(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    return *std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
        return a->priority < b->priority;
    });
}

Process* doRAND(std::vector<Process*>& readyList) {
    if (readyList.empty()) return nullptr;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, readyList.size() - 1);
    int index = dis(gen);
    return *(readyList.begin() + index);
}