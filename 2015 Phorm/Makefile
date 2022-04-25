CXX = g++
CXXFLAGS = -g -O2 -Wall -Wextra -Wunused -Wfloat-equal -Wvla -Wshadow -Weffc++ -Wstrict-aliasing -c
SOURCE = ./src
OBJ = ./obj
OUTPUT = ./output


all: prepare test


clean:
	@rm -rf $(OBJ)
	@rm -rf $(OUTPUT)


prepare:
	@mkdir -p $(SOURCE)
	@mkdir -p $(OBJ)
	@mkdir -p $(OUTPUT)


test:   main.o \
        contiguous_buffer.o \
        contiguous_buffer_tests.o \
        segmented_buffer.o \
        segmented_buffer_tests.o
	$(CXX) \
        $(OBJ)/main.o \
        $(OBJ)/contiguous_buffer.o \
        $(OBJ)/contiguous_buffer_tests.o \
        $(OBJ)/segmented_buffer.o \
        $(OBJ)/segmented_buffer_tests.o \
        -o $(OUTPUT)/test


main.o: $(SOURCE)/contiguous_buffer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/main.cpp -o $(OBJ)/main.o


contiguous_buffer.o: $(SOURCE)/contiguous_buffer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/contiguous_buffer.cpp -o $(OBJ)/contiguous_buffer.o  


contiguous_buffer_tests.o: $(SOURCE)/unit_tests.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/contiguous_buffer_tests.cpp -o $(OBJ)/contiguous_buffer_tests.o  


segmented_buffer.o: $(SOURCE)/segmented_buffer.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/segmented_buffer.cpp -o $(OBJ)/segmented_buffer.o  


segmented_buffer_tests.o: $(SOURCE)/unit_tests.h
	$(CXX) $(CXXFLAGS) $(SOURCE)/segmented_buffer_tests.cpp -o $(OBJ)/segmented_buffer_tests.o
