#include "../../include/algorithms.h"

Process* doSRTF(std::vector<Process*>& readyList, Process* runningProcess) {
    if (!readyList.empty()) {
        auto it = std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
            return a->remainingTimeCPU < b->remainingTimeCPU;
        });
        if (runningProcess != nullptr) {
            if ((*it)->remainingTimeCPU < runningProcess->remainingTimeCPU) {
                return *it;
            }
        }
        else {
            return *it;
        }
    }
    return nullptr;
}

Process* doPP(std::vector<Process*>& readyList, Process* runningProcess) {
    if (!readyList.empty()) {
        auto it = std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
            return a->priority < b->priority;
        });
        if (runningProcess != nullptr) {
            if ((*it)->priority < runningProcess->priority) {
                return *it;
            }
        }
        else {
            return *it;
        }
    }
    return nullptr;
}

Process* doRR(std::vector<Process*>& readyList, Process* runningProcess, int& counter, const int& quantum) {
    if (runningProcess == nullptr && !readyList.empty()) {
        counter = 0;
        return *readyList.begin();
    }
    else if (runningProcess != nullptr) {
        ++counter;
        if (counter >= quantum) {
            counter = 0;
            if (!readyList.empty()) {
                return *readyList.begin();
            }
        }
    }
    return nullptr;
}