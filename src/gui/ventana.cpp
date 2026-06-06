#include "gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Incluimos GLFW nativo. 
#include <GLFW/glfw3.h> 

static GLFWwindow* window = nullptr;

bool runInterface() {
    // 1. Inicializar GLFW
    if (!glfwInit()) {
        std::fprintf(stderr, "Error al inicializar GLFW\n");
        return false;
    }

    // Configurar versión de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Crear la ventana del sistema
    window = glfwCreateWindow(1280, 720, "Simulador de Planificación de Procesos", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Habilitar V-Sync (Sincronización vertical)

    // 3. Inicializar el Contexto de Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Habilitar navegación por teclado

    ImGui::StyleColorsDark();

    // 4. Inicializar plataformas de Renderizado (Backends)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return true;
}

bool isClosing() {
    return glfwWindowShouldClose(window);
}

void renderInterface(Simulator& simulador) {
    // Escuchar eventos del sistema (mouse, teclado, cambiar tamaño de ventana)
    glfwPollEvents();

    // Iniciar el nuevo frame de ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Panel de Control del Simulador
    static float time = 1;
    static bool isAuto = false;

    if(isAuto) simulador.runTick(time);

    ImGui::Begin("Reloj");
    ImGui::Text("Estado del Reloj del Sistema: %d ticks", simulador.actualTime);
    ImGui::End();
    
    if(!isAuto && !simulador.isSimulating())
    {
        ImGui::Begin("Panel de Control");
        ImGui::Text("Establecer duracion del Tick (seg)"); 
        ImGui::SameLine();
        if(ImGui::InputFloat("##", &time)) {
            if(time<0) time = 0;
        };

        static int algorithm = 0;
        static int quantum = 1; 

        ImGui::Text("Selecciona el Algoritmo de Planificación:");
        if(ImGui::RadioButton("FCFS", &algorithm, 0)) {
            simulador.algorithm = FCFS;
        };
        ImGui::SameLine();
        if(ImGui::RadioButton("SJF", &algorithm, 1)) {
            simulador.algorithm = SJF;
        };
        ImGui::SameLine();
        if(ImGui::RadioButton("NPP", &algorithm, 2)) {
            simulador.algorithm = NPP;
        };
        ImGui::SameLine();
        if(ImGui::RadioButton("RAND", &algorithm, 3)) {
            simulador.algorithm = RAND;
        };
        ImGui::SameLine();
        if(ImGui::RadioButton("SRTF", &algorithm, 4)) {
            simulador.algorithm = SRTF;
        };
        ImGui::SameLine();
        if(ImGui::RadioButton("PP", &algorithm, 5)) {
            simulador.algorithm = PP;
        };
        ImGui::SameLine();
        if(ImGui::RadioButton("RR", &algorithm, 6)) {
            simulador.algorithm = RR;
        };
        if(algorithm == 6) {
            ImGui::Text("Ingrese el quantum a utilizar:");
            if (ImGui::InputInt("##quantumInput", &quantum)) {
                if(quantum<1) quantum = 1;
                simulador.quantum = quantum;
            };
        }
        ImGui::End();
    }

    //Simular
    ImGui::Begin("Simular");
    if(!isAuto)
    {
        if (ImGui::Button("Siguiente Paso (Tick)")) {
            simulador.runTick(0);
        }
        ImGui::SameLine();
        if (ImGui::Button("Simular todo")) {
            isAuto = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            simulador.reset();
        }
    }
    else
    {
        if (ImGui::Button("Stop (Dejar de Simular)")) {
            isAuto = false;
        }
    }
    ImGui::End();

    // Opciones de editor
    if(!simulador.isSimulating())
    {
        static int arriveTime = 0;
        static int timeCPU = 1;
        static int timeIO = 0;
        static int numCycles = 0;
        static int priority = 1;

        ImGui::Begin("Opciones de modelado");
        ImGui::InputInt("Tiempo de Llegada", &arriveTime);
        if (ImGui::InputInt("Tiempo de CPU", &timeCPU)) {
            if(timeCPU<1) timeCPU = 1;
        };
        if (ImGui::InputInt("Tiempo de E/S", &timeIO)) {
            if(timeIO<0) timeIO = 0;
        };
        if (ImGui::InputInt("Numero de ciclos", &numCycles)) {
            if(numCycles<0) numCycles = 0;
        };
        if (ImGui::InputInt("Prioridad", &priority)) {
            if(priority<1) priority = 1;
        };
        if (ImGui::Button("Crear Proceso")) {
            simulador.addProcess(arriveTime,timeCPU,timeIO,priority,numCycles);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            arriveTime = 0;
            timeCPU = 1;
            timeIO = 0;
            numCycles = 0;
            priority = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Random")) {
            static std::random_device rd;
            static std::mt19937 gen(rd());

            std::uniform_int_distribution<> distArrive(0, 60);
            std::uniform_int_distribution<> distCPU(1, 60);
            std::uniform_int_distribution<> distIO(0, 60);
            std::uniform_int_distribution<> distCycles(0, 5);
            std::uniform_int_distribution<> distPriority(0, 60);

            arriveTime = distArrive(gen);
            timeCPU = distCPU(gen);
            timeIO = distIO(gen);
            numCycles = distCycles(gen);
            priority = distPriority(gen);
        }
        ImGui::End();
    }

    // Lista de procesos
    ImGui::Begin("Lista de Procesos");
    if (ImGui::BeginTable("TablaProcesos", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Tiempo de Llegada");
        ImGui::TableSetupColumn("Ráfaga CPU restante");
        ImGui::TableSetupColumn("Ráfaga IO restante");
        ImGui::TableSetupColumn("Prioridad");
        ImGui::TableSetupColumn("Estado");
        ImGui::TableSetupColumn("Numero de Ciclos");
        ImGui::TableSetupColumn("##");
        ImGui::TableHeadersRow();

        for (const Process* process : simulador.getProcesses()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process->id);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", process->arrivalTime);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%d", process->remainingTimeCPU);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%d", process->remainingTimeIO);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%d", process->priority);
            ImGui::TableSetColumnIndex(5); ImGui::Text("%s", process->getState().c_str());
            ImGui::TableSetColumnIndex(6); ImGui::Text("%d", process->remainingCycles);
            if(!simulador.isSimulating()) {
                ImGui::TableSetColumnIndex(7); ImGui::PushID(process->id); 
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
                if (ImGui::Button("Eliminar")) {
                    simulador.deleteInitialProcess(process->id);
                    ImGui::PopStyleColor(2);
                    ImGui::PopID();
                    break;
                }
                ImGui::PopStyleColor(2);
                ImGui::PopID();
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();

    ImGui::Begin("Cola de Listos");
    if (ImGui::BeginTable("TablaListos", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Prioridad");
        ImGui::TableHeadersRow();

        for (const Process* process : simulador.readyList) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process->id);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", process->priority);
        }
        ImGui::EndTable();
    }
    ImGui::End();

    ImGui::Begin("Cola de Bloqueados");
    if (ImGui::BeginTable("TablaBloqueados", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Rafaga IO restante");
        ImGui::TableHeadersRow();

        for (const Process* process : simulador.blockedList) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process->id);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", process->remainingTimeIO);
        }
        ImGui::EndTable();
    }
    ImGui::End();

    ImGui::Begin("Cola de Terminados");
    if (ImGui::BeginTable("TablaTerminados", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Tiempo de llegada");
        ImGui::TableSetupColumn("Tiempo final");
        ImGui::TableHeadersRow();

        for (const Process* process : simulador.terminatedList) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process->id);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", process->arrivalTime);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%d", process->completionTime);
        }
        ImGui::EndTable();
    }
    ImGui::End();

    if(simulador.checkExit()) isAuto = false; 

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void freeInterface() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}