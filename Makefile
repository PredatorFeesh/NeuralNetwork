CXX=g++
FLAGS=-std=c++17
DEBUG=-g

MAIN=main.cpp
OUTNAME=a

OUTPUT=-o $(OUTNAME)

CPPDIR=cpp

install: $(MAIN)
	$(CXX) $(MAIN) $(CPPDIR)/*.cpp $(FLAGS) $(OUTPUT)

make_test: $(MAIN)
	$(CXX) mat_test.cpp cpp/Matrix.cpp $(FLAGS) $(OUTPUT)

debug: $(MAIN)
	$(CXX) $(MAIN) $(CPPDIR)/*.cpp $(FLAGS) $(OUTPUT) $(DEBUG)

run: $(OUTNAME)
	./$(OUTNAME)