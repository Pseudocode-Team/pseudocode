#pragma once
#include <iostream>

void print(std::string message) {
	std::cout << message << std::endl;
}

void print(int message) {
	std::cout << message << std::endl;
}

#include <string>
template <class Container>
void split(const std::string& str, Container& cont, char delim = ' '){
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}
