#include "../../include/algorithms.h"

Process* FCFS(std::vector<Process*>& readyList) {
    if (readyList.empty()) {
        return nullptr;
    }
    Process* process = readyList.front();
    process->state = RUNNING;
    readyList.erase(readyList.begin());
    return process;
}