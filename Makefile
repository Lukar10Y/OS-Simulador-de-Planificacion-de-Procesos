# Variables del Compilador
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
OBJDIR = obj
SRCDIR = src
ALGODIR = src/algorithms
GUIDIR = src/gui

# VPATH le dice a 'make' dónde buscar los archivos .cpp automáticamente
VPATH = $(SRCDIR):$(ALGODIR):$(GUIDIR)

# Detección del Sistema Operativo
ifeq ($(OS),Windows_NT)
    RM = del /q
    RMDIR = rmdir /s /q
    MKDIR = if not exist $(OBJDIR) mkdir $(OBJDIR)
    TARGET = simulador.exe
    LIBS = -Llib -lglfw3 -lopengl32 -lgdi32
else
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p $(OBJDIR)
    TARGET = simulador
    LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

# 1. Buscar todos los archivos fuentes (.cpp) con sus rutas completas
SOURCES_SRC = $(wildcard $(SRCDIR)/*.cpp)
SOURCES_ALGO = $(wildcard $(ALGODIR)/*.cpp)
SOURCES_GUI = $(wildcard $(GUIDIR)/*.cpp)

# 2. Mapear los .cpp a .o planos en la carpeta obj/
OBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES_SRC) $(SOURCES_ALGO) $(SOURCES_GUI)))

# Declarar comandos que no representan archivos físicos
.PHONY: all clean

# Regla principal
all: $(OBJDIR) $(TARGET)

# Enlace final
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# REGLA ÚNICA: Gracias a VPATH, una sola regla procesa cualquier .cpp de cualquier carpeta
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear directorio de objetos si no existe
$(OBJDIR):
	@$(MKDIR)

# Regla de Limpieza (Incluye imgui.ini de forma segura)
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJDIR) $(RMDIR) $(OBJDIR)
	@if exist $(TARGET) $(RM) $(TARGET)
	@if exist imgui.ini $(RM) imgui.ini
else
	@$(RMDIR) $(OBJDIR)
	@$(RM) $(TARGET)
	@$(RM) imgui.ini
endif
	@echo Proyecto limpio.