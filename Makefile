CXX=g++
FLAGS=-std=c++17
DEBUG=-g

MAIN=main.cpp
OUTNAME=a

OUTPUT=-o $(OUTNAME)

CPPDIR=cpp

install: $(MAIN)
	$(CXX) $(MAIN) $(CPPDIR)/*.cpp $(FLAGS) $(OUTPUT)

test: $(MAIN)
	$(CXX) tests.cpp cpp/* tests/* $(FLAGS) $(OUTPUT)

debug: $(MAIN)
	$(CXX) $(MAIN) $(CPPDIR)/*.cpp $(FLAGS) $(OUTPUT) $(DEBUG)

run: $(OUTNAME)
	./$(OUTNAME)