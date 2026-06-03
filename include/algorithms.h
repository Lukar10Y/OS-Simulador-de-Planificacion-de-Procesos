#pragma once

#include "process.h"
#include <vector>
#include <algorithm>
#include <random>

// No expulsivos
Process* doFCFS(std::vector<Process*>& readyList);
Process* doSJF(std::vector<Process*>& readyList);
Process* doNPP(std::vector<Process*>& readyList);
Process* doRAND(std::vector<Process*>& readyList);