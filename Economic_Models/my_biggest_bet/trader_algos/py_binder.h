// utils/PyBinder.h
#pragma once

#include <pybind11/embed.h>
#include <functional>
#include <string>
#include "../utils/trade_message.h"
#include "../utils/market_dataframe.h"

namespace py = pybind11;

class PyBinder {
public:
    // Call this once at the start of main()
    static void initialize_python() {
        py::initialize_interpreter();
        // Add the current path to the Python module search path
        // This allows Python to find our 'strategies.py' script.
        py::exec(R"(
            import sys
            sys.path.append('.')
        )");

        py::exec(R"(
        import sys
        import os
        print("Python Path:", sys.path)
        print("Current Working Directory:", os.getcwd())
        sys.path.append('.')
        )");
    }

    // Call this once at the end of main()
    static void cleanup_python() {
        py::finalize_interpreter();
    }
    
    // This is the key function. It loads a Python function and wraps it
    // in a C++ std::function that the Trader class can use.
    static std::function<Trades::Trade_Message(const MarketDataframe&)>
    get_py_trade_func(const std::string& module_name, const std::string& func_name) {
        
        // Return a C++ lambda function that captures the Python function
        return [module_name, func_name](const MarketDataframe& market_data) -> Trades::Trade_Message {
            try {
                // Import the python module (e.g., "strategies")
                py::module_ module = py::module_::import(module_name.c_str());

                // Get the function from the module (e.g., "moving_avg_trader")
                py::object py_func = module.attr(func_name.c_str());

                // Call the Python function, passing the C++ market_data object.
                // pybind11 handles the conversion to a Python object automatically.
                py::object result = py_func(market_data);

                // Convert the Python object result back to a C++ Trade_Message.
                // pybind11 handles this automatically as well.
                return result.cast<Trades::Trade_Message>();

            } catch (const py::error_already_set& e) {
                // If the Python code throws an error, print it to C++ stderr
                std::cerr << "Python error: " << e.what() << std::endl;
                // Return a default "do nothing" trade
                return {"", Trades::MARKET_BUY, 0, 0};
            }
        };
    }
};
 
