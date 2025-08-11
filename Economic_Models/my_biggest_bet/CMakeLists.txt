cmake_minimum_required(VERSION 3.15)
project(StockMarketSim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

# Add pybind11
add_subdirectory(external/pybind11)

# Define C++ executable
add_executable(
    market_sim
    main.cpp
    stock.cpp
    exchange.cpp
    utils/industry.cpp

    # Add other .cpp files here if they are not header-only
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# Link pybind11 to executable
# This adds include directories and necessary linker flags
target_link_libraries(market_sim PRIVATE pybind11::embed OpenMP::OpenMP_CXX)

# Python Module
# Defines the C++ bindings that Python can import.
# Creates a file like `trader_bindings.so` or `trader_bindings.pyd`
pybind11_add_module(
    trader_bindings  # The name Python will use to import
    src/pybinder.cpp # File containing binding code
)


# To build:
# mkdir build
# cd build
# cmake ..
# make
