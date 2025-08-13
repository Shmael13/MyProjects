cmake_minimum_required(VERSION 3.15)
project(StockMarketSim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Python3_ROOT_DIR "/usr")
set(Python3_FIND_VERSION "3.10")

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

find_package(OpenMP REQUIRED)

# Add pybind11
add_subdirectory(external/pybind11)

# Define C++ executable
add_executable(
    market_sim
    main.cpp
    utils/stock.cpp
    utils/exchange.cpp
    utils/industry.cpp
    # Add other .cpp files here
)

# Link SFML and OpenMP
target_link_libraries(market_sim PRIVATE
    ${Python3_LIBRARIES}
    OpenMP::OpenMP_CXX
    pybind11::embed
)

# Python Module (pybind11)
pybind11_add_module(
    trader_bindings
    src/pybinder.cpp

)
target_link_libraries(trader_bindings PRIVATE
    ${Python3_LIBRARIES}
)

# To build:
# mkdir build
# cd build
# cmake ..
# make
