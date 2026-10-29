INPUT = entrada.in

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2 -Isrc
FONTES = $(shell find src -name '*.cpp')
CABECALHOS = $(shell find src -name '*.hpp')
PROGRAMA = programa

.PHONY: build run test clean

build: $(PROGRAMA)

$(PROGRAMA): $(FONTES) $(CABECALHOS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAMA) $(FONTES)

# O "@" impede que o make ecoe o comando na saída, o que quebraria a comparação com a saída esperada.
run: $(PROGRAMA)
	@./$(PROGRAMA) $(INPUT)

test: $(PROGRAMA)
	@falhas=0; \
	for entrada in testes/*.in; do \
		esperado=$${entrada%.in}.out; \
		if ./$(PROGRAMA) $$entrada | diff -q - $$esperado > /dev/null; then \
			echo "ok     $$entrada"; \
		else \
			echo "falhou $$entrada"; \
			falhas=$$((falhas + 1)); \
		fi; \
	done; \
	test $$falhas -eq 0

clean:
	rm -f $(PROGRAMA)
