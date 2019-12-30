CXX=g++
FLAGS=-std=c++17 
DEBUG=-g

MAIN=main.cpp
OUTPUT=-o out

CPPDIR=cpp

install: $(MAIN)
	$(CXX) $(MAIN) $(CPPDIR)/*.cpp $(FLAGS) $(OUTPUT)

debug: $(MAIN)
	$(CXX) $(MAIN) $(CPPDIR)/*.cpp $(FLAGS) $(OUTPUT) $(DEBUG)
