pdc: main.o
	g++ -o pdc main.cpp

main.o: main.cpp
	g++ -c main.cpp


