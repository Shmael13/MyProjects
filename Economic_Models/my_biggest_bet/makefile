#Compilers
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wconversion -Wsign-conversion -Weffc++ -std=c++23 

EXEC_FLAGS = -O3 

# Python flags
PYTHON_LDFLAGS = $(shell pkg-config --libs python3-embed 2>/dev/null || pkg-config --libs python3) -lpython3.10
PY_CFLAGS = $(shell pkg-config --cflags python3)

#Target Executable
TARGET = main

#Source Files
SRCS = $(wildcard *.cpp utils/*.cpp)

#Header files
HEADER = $(wildcard *.h utils/*.h)

#Object files
OBJS = $(SRCS:.cpp=.o)

#Defualt Target
all:$(TARGET)

#Link obj files to executable
$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) $(EXEC_FLAGS) $^ -o $@ $(PYTHON_LDFLAGS)

%.o: %.cpp $(HEADER) 
	$(CXX) $(CXXFLAGS) $(PY_CFLAGS) $(EXEC_FLAGS) -c $< -o $@ 

clean:
	rm -f $(OBJS) $(TARGET)
	
