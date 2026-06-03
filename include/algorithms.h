#pragma once

#include "process.h"
#include <vector>
#include <algorithm>

// No expulsivos
Process* doFCFS(std::vector<Process*>& readyList);
Process* doSJF(std::vector<Process*>& readyList);
Process* doNPP(std::vector<Process*>& readyList);