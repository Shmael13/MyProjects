#Compilers
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wconversion -Wsign-conversion -Weffc++

#Target Executable
TARGET = main

#Source Files
SRCS = main.cpp stock.cpp utils/industry.cpp

#Header files
HEADER = stock.h utils/industry.h

#Object files
OBJS = $(SRCS:.cpp=.o)

#Defualt Target
all:$(TARGET)

#Link obj files to executable
$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(HEADER) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	
