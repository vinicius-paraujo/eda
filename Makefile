INPUT = entrada.in

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2 -Isrc
FONTES = $(shell find src -name '*.cpp')
CABECALHOS = $(shell find src -name '*.hpp')
PROGRAMA = programa

.PHONY: build run clean

build: $(PROGRAMA)

$(PROGRAMA): $(FONTES) $(CABECALHOS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAMA) $(FONTES)

run: $(PROGRAMA)
	./$(PROGRAMA) $(INPUT)

clean:
	rm -f $(PROGRAMA)
