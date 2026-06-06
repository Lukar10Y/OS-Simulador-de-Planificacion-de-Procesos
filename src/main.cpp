/******************************************************************************
 * @file main.cpp
 * @brief Punto de entrada del Simulador de Planificación de Procesos (OS)
 * @author Francisco Ochoa (C.I: 30.189.260)
 * @date Junio 2026
 * @note Proyecto académico de Sistemas Operativos - Interfaz con ImGui/GLFW
 *****************************************************************************/

#include "simulator.h"
#include "gui.h"

int main() {
    // 1. Instanciar tu objeto de lógica de simulación
    Simulator simulador;
    
    simulador.loadProcesses(); 

    // 2. Inicializar el motor de la interfaz gráfica
    if (!runInterface()) {
        std::cerr << "Error crítico: No se pudo arrancar la interfaz gráfica." << std::endl;
        return -1;
    }

    // 3. Bucle Principal de la Aplicación (Corre a los FPS de tu monitor)
    while (!isClosing()) {
        // En cada frame renderizamos la interfaz y le pasamos los datos del simulador
        renderInterface(simulador);
    }

    // 4. Limpieza final antes de salir
    freeInterface();
    
    return 0;
}