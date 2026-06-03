# Variables del Compilador
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
OBJDIR = obj
SRCDIR = src
ALGODIR = src/algorithms

# Detección del Sistema Operativo
ifeq ($(OS),Windows_NT)
    # Configuración para Windows
    RM = del /q
    RMDIR = rmdir /s /q
    MKDIR = if not exist $(OBJDIR) mkdir $(OBJDIR)
    TARGET = simulador.exe
    FIX_PATH = $(subst /,\,$1)
else
    # Configuración para Linux / macOS
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p $(OBJDIR)
    TARGET = simulador
    FIX_PATH = $1
endif

# Buscar archivos fuentes y objetos
SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(ALGODIR)/*.cpp)
OBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

# Reglas de compilación
all: $(OBJDIR) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(ALGODIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	@$(MKDIR)

# Regla de Limpieza multiplataforma
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJDIR) $(RMDIR) $(OBJDIR)
	@if exist $(TARGET) $(RM) $(TARGET)
else
	@$(RMDIR) $(OBJDIR)
	@$(RM) $(TARGET)
endif
	@echo Proyecto limpio.