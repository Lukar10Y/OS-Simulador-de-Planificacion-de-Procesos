#include "process.h"

int main() {
    std::cout << "Simulador de Planificacion de Procesos Inicializado" << std::endl;
    Process p1(1, 0, 5, 3, 1);
    p1.print();
    return 0;
}