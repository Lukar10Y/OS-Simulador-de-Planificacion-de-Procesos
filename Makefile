# Variables de configuración
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
TARGET = simulador.exe

# Directorios
SRCDIR = src
ALGODIR = src/algoritmos
OBJDIR = obj

# Buscar automáticamente todos los archivos .cpp
SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(ALGODIR)/*.cpp)

# Generar los nombres de los archivos .o equivalentes dentro de la carpeta obj/
OBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

# Regla principal (la que se ejecuta por defecto al escribir 'make')
all: $(OBJDIR) $(TARGET)

# Regla para enlazar los archivos objeto y crear el ejecutable final
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar los archivos .cpp de la raíz de src/ a .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para compilar los archivos .cpp de la carpeta src/algoritmos/ a .o
$(OBJDIR)/%.o: $(ALGODIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear la carpeta 'obj' si no existe en Windows
$(OBJDIR):
	@if not exist $(OBJDIR) mkdir $(OBJDIR)

# Regla para limpiar los archivos compilados en Windows
clean:
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(TARGET) del /q $(TARGET)
	@echo Proyecto limpio.