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
    ImGui::Begin("Panel de Control");
    
    ImGui::Text("Estado del Reloj del Sistema: %d u.t.", simulador.actualTime); 
    
    if (ImGui::Button("Siguiente Paso (Tick)")) {
        simulador.runTick();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Resetear Simulador")) {
        //simulador.reiniciar();
    }
    
    ImGui::End();

    // Lista de procesos
    ImGui::Begin("Lista de Procesos");
    if (ImGui::BeginTable("TablaProcesos", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("T. Llegada");
        ImGui::TableSetupColumn("Ráfaga CPU restante");
        ImGui::TableSetupColumn("Ráfaga IO restante");
        ImGui::TableSetupColumn("Prioridad");
        ImGui::TableSetupColumn("Estado");
        ImGui::TableHeadersRow();

        for (const Process* process : simulador.backupList) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", process->id);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", process->arrivalTime);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%d", process->remainingTimeCPU);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%d", process->remainingTimeIO);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%d", process->priority);
            ImGui::TableSetColumnIndex(5); ImGui::Text("%s", process->getState().c_str());
        }
        ImGui::EndTable();
    }
    ImGui::End();

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