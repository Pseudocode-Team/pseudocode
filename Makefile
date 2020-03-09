SRC_DIR := ./src
OBJ_DIR := ./.obj

pdc: $(OBJ_DIR)/main.o
	g++ -o pdc $(OBJ_DIR)/main.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	-mkdir -p $(OBJ_DIR)
	g++ -c -o $@ $<


