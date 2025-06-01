#Compilers
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wconversion -Wsign-conversion -Weffc++ -std=c++20

#Target Executable
TARGET = main

#Source Files
SRCS = main.cpp stock.cpp utils/industry.cpp exchange.cpp

#Header files
HEADER = stock.h utils/industry.h exchange.h

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
	
