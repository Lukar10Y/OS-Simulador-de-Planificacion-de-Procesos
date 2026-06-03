#include "../../include/algorithms.h"

Process* doSRTF(std::vector<Process*>& readyList, Process* runningProcess) {
    if (!readyList.empty()) {
        auto it = std::min_element(readyList.begin(), readyList.end(), [](Process* a, Process* b) {
            return a->remainingTimeCPU < b->remainingTimeCPU;
        });
        Process* newProcess = *it;
        if (runningProcess != nullptr) {
            if (newProcess->remainingTimeCPU < runningProcess->remainingTimeCPU) {
                readyList.erase(it);
                runningProcess->state = READY;
                readyList.push_back(runningProcess);
                newProcess->state = RUNNING;
                return newProcess;
            }
        }
        else {
            readyList.erase(it);
            newProcess->state = RUNNING;
            return newProcess;
        }
    }
    return runningProcess;
}