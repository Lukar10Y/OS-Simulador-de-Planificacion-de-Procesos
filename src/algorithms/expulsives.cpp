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