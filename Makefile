SRC_DIR := ./src
OBJ_DIR := ./.obj

test: pdc
	./pdc test

pdc: $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h
	g++ -std=c++11 -o pdc $(SRC_DIR)/*.cpp
