## Description
This directory contains an implementation fo a Multi-Agent Market simulation.
You can customize and design your own trading strategies using C++ or Python, and compare results and performance across different strategies. 

## Structure
This is the structure:
- `external/pybind11` contains the pybind11 library, which enables the creation of python bindings
-  `src` contains implementations of the python bindings for this specific project
-  `trader_algos` contains trading algorithms which can be implemented in either python or c++. Functions to visualize the data can also be defined here
-  `utils` contains implementations of various utilities used within the program, including implementations for the stocks, traders, exchange, and the messages sent between these entities and the trading algorithms.
-  `main.cpp` contains an example implementation of using this framework. You can edit this document to change the initial parameters of the experiment.
- `makefile` is able to create a compiled version of the program __'warning, this may not work!, as this was primarily used for debugging - it doesn't create the python bindings, we need to use cmake for that'__
- `CMakeLists.txt` contains the cmake file which is used to create the project

## Instructions to run
To run this project: 
1.  Clone the repository.
2.  Create a buildfile with
  - mkdir build
  - cmake ..
  - make
3. Run with ./market_sim
(These instructions are for Linux/Unix systems, to run in Windows just do the equivalent of the above, or do the instructions above in WSL)

## How to make new traders
You can make new traders within the trader_algos file. There are two options - making traders in C++, and making traders in Python. _Note that if you make traders in C++, the program will run faster_

1. Python
Python traders can be made by creating a function that takes as input a trader_bindings.MarketDataframe, and outputs a trader_bindings.Trade_Message, where trader_bindings are the custom python bindings implemented within the project. I have included examples on how you can access the datatypes within the MarketDataframe, and what a properly formatted Trade_Message looks like. These examples are placed within `trader_algos/strategies.py`
You can add these traders within the simulation through the `main.cpp` code, by adding them to the traders vector, and specifying the trader's name, python module, function, and the initial cash allocated to the trader as shown in the examples.
You can initialize multiple traders who use the same strategy.*

2. C++
C++ traders can be created and utilized within the `trader_algos/trader_algos.h` file. They are functions that take as input a MarketDataframe, ans return a Trades::Trade_Message. I have implemented various example traders.
You can add these traders within the simulation through the `main.cpp` code, by adding them to the traders vector, and specifying the trader's name, function, and the initial cash allocated to the trader as shown in the examples.
You can initialize multiple traders who use the same strategy.*

* Make sure all the python modules you create are within the `trader_algos` directory, and the C++ strategies are within `trader_algos/trader_algos.h` 
* __NOTE: global variables in may be shared between traders. This is a feature and not a bug, just in case someone wants to simulate a strategy whereby traders communicate with each other using a shared global variable. Do not use global variables (or only access a global variable solely within a specific strategy, and only use that strategy once) if you do not want this feature__

## How to visualize data
Data can be visualized using the display_dataframe.py module. This also takes as input a market_dataframe, and doesn't need to output anything.




## Further Improvements
- Visualizing the data from the traders' perspective i.e figuring out what trades each trader made isn't currently possible, but should be fairly simple to implement
- Optimize the program using threading, and figure out some way to be more memory efficient
- Implement a more sophisticated routine for communication between traders
- Find some way to implement `events`, i.e mass buys, sells, or shocks, which could happen to either certain stocks, or certain industries
- Make it so that certain traders are more priviledged than others, in that they are able to get information about the next datastep within the market before others, mimicing how HF traders are slightly ahead of others in getting market info
