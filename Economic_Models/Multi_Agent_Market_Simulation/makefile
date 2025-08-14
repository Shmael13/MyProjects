#Compilers
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wconversion -Wsign-conversion -Weffc++ -std=c++23 
EXEC_FLAGS = -O3 -fopenmp

# OS Detection
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    EXE = .exe
    SEP = \\
    NULL_DEV = NUL
else
    UNAME_S := $(shell uname -s)
    RM = rm -f
    EXE =
    SEP = /
    NULL_DEV = /dev/null
endif

# Python flags
PYTHON_LDFLAGS = $(shell pkg-config --libs python3-embed 2>/dev/null || pkg-config --libs python3) -lpython3.10
PY_CFLAGS = $(shell pkg-config --cflags python3)

#Target Executable
TARGET = main$(EXE)

#Source Files
SRCS = $(wildcard *.cpp utils$(SEP)*.cpp trader_algos$(SEP)*.cpp src$(SEP)*.cpp)

#Header files
HEADER = $(wildcard *.h utils$(SEP)*.h trader_algos$(SEP)*.h src$(SEP)*.h)

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
	$(RM) $(OBJS) $(TARGET) 2>$(NULL_DEV)
