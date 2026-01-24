# --- CONFIGURAÇÃO DO COMPILADOR ---
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O2
LDFLAGS  := 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := KSPTool
INCLUDE  := -Iinclude/
SRC      := $(wildcard *.cpp) 

# --- AUTOMAGIA DE OBJETOS ---
OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# --- REGRAS DE COMPILAÇÃO ---

# Regra padrão: compilar tudo
all: build $(APP_DIR)/$(TARGET)

# Cria as pastas necessárias
build:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(APP_DIR)

# Linkagem final (Cria o executável)
$(APP_DIR)/$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build Complete! Executable is at $(APP_DIR)/$(TARGET)"

# Compilação de cada arquivo .cpp para .o
$(OBJ_DIR)/%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# --- UTILITÁRIOS ---

# Limpar tudo (Reset)
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD)

# Rodar o programa
run: all
	@echo "Running $(TARGET)..."
	@./$(APP_DIR)/$(TARGET)

# Regra para evitar confusão com arquivos que tenham nomes iguais a essas regras
.PHONY: all build clean debug release run