#ifndef GUI_H
#define GUI_H

#include "simulator.h"

// Inicializa GLFW, OpenGL3 y Dear ImGui. Retorna true si todo salió bien.
bool runInterface();

// Renderiza la interfaz gráfica del simulador en cada frame
// Pasamos el simulador por referencia para mostrar y modificar sus datos en tiempo real
void renderInterface(Simulator& simulador);

// Libera la memoria y destruye los contextos gráficos al cerrar el programa
void freeInterface();

// Función auxiliar para verificar si la ventana se debe cerrar
bool isClosing();

#endif // GUI_H