CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17
SRC = interval_map.cpp
OUT = interval_map

# Verifica el sistema operativo
UNAME_S := $(shell uname -s)

# Ajusta el nombre del archivo de salida en función del sistema
ifeq ($(UNAME_S),Windows_NT)
	OUT := $(OUT).exe
endif

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
