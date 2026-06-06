#pragma once

#include "algorithms.h"
#include <chrono>
#include <thread>

enum AlgorithmType {
    FCFS = 0,
    SJF = 1,
    NPP = 2,
    RAND = 3,
    SRTF = 4,
    PP = 5,
    RR = 6
};

struct Simulator {
    //Reloj
    int actualTime;

    // Colas

    std::vector<Process*> initialList;
    std::vector<Process*> readyList;
    std::vector<Process*> blockedList;
    std::vector<Process*> terminatedList;
    std::vector<Process*> backupList;
    // Proceso actual que esta usando el CPU
    Process* runningProcess;
    // Algoritmo a utilizar
    AlgorithmType algorithm;

    int quantum;
    int counterQuantum;
    int counterID;

    // Ticks de ocio
    int idleTime;

    std::vector<float> historyCPU;

    Simulator();
    ~Simulator();

    void addProcess(int arrivalTime, int timeCPU, int timeIO, int priority, int cycles = 1);
    void loadProcesses();

    // Realizar simulacion completa imprimiendo por consola
    void runInConsole(const float& time);
    void runTickInConsole(const float& time);

    void runTick(const float& time);

    // Metricas tomando como referencia la terminatedList

    void getFinalMetrics();
    void calcFinalMetrics();
    void getFinalAverageMetrics();

    //  Imprimir colas segun el tick actual
    void print();
    
    //
    void subsActualProcess(Process* process);
    void deleteInitialProcess(const int& id);
    void reset();

    bool checkExit() const;
    bool isSimulating() const;

    // Actualizar cola del estado indicado mostrando mensajes por consola
    void updateQueueInConsole(State state);
    // Actualizar cola del estado indicado
    void updateQueue(State state);

    // Segun el algoritmo indicado en algorithm, va a buscar en readyList cualquier
    // proceso que cumpla con los requerimientos y pueda montarse de inmediato en el CPU
    Process* doAlgorithm();

    // Getters

    std::vector<Process*> getProcesses();

    // Setters

};