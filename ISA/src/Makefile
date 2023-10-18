#  Autor: Petr Kaška
#  Datum: 2. října 2023
EXE = dns

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -static-libstdc++


SRC = dns.cpp customErr.cpp string.cpp
OBJ = $(SRC:.cpp=.o)

$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) dns.o customErr.o string.o -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXE)

.PHONY: all clean

all: $(EXE)

test: dns
	python3 test.py
