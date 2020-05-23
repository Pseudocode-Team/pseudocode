SRC_DIR := ./src
OBJ_DIR := ./.obj

test: pdc
	./pdc

pdc: $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h
	g++ -o pdc $(SRC_DIR)/*.cpp
