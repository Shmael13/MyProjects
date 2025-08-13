// src/pybinder.cpp
#include <pybind11/chrono.h> // Needed for time conversions
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Needed for automatic conversion of std::vector, etc.

#include "../utils/market_dataframe.h"
#include "../utils/stock_frame.h"
#include "../utils/trade_message.h"

namespace py = pybind11;

// The PYBIND11_MODULE macro creates a function that will be called when
// the Python interpreter imports the module.
// The first argument is the module name (must match CMake target name)
// The second argument, 'm', is a py::module_ object that is the main interface
PYBIND11_MODULE(trader_bindings, m) {
  m.doc() = "Bindings for the C++ stock market simulator"; // Optional module
                                                           // docstring

  // --- Bind Enums ---
  py::enum_<Trades::TradeType>(m, "TradeType")
      .value("MARKET_BUY", Trades::TradeType::MARKET_BUY)
      .value("LIMIT_BUY", Trades::TradeType::LIMIT_BUY)
      .value("MARKET_SELL", Trades::TradeType::MARKET_SELL)
      .value("LIMIT_SELL", Trades::TradeType::LIMIT_SELL)
      .export_values(); // Makes the enum members available in the module's
                        // namespace

  // --- Bind Structs/Classes ---
  // py::class_ creates bindings for a C++ class or struct
  py::class_<StockFrame>(m, "StockFrame")
      .def(py::init<>()) // Expose the default constructor
      // Expose members as read/write properties in Python
      .def_readwrite("ticker", &StockFrame::ticker)
      .def_readwrite("industry_name", &StockFrame::industry_name)
      .def_readwrite("curr_price", &StockFrame::curr_price)
      .def_readwrite("vol_traded", &StockFrame::vol_traded)
      .def_readwrite("num_stocks", &StockFrame::num_stocks)
      .def_readwrite("recording_time", &StockFrame::recording_time);
  // Note: pybind11/chrono.h will automatically handle long long -> Python
  // datetime

  py::class_<Trades::Trade_Message>(m, "Trade_Message")
      .def(py::init<>())
      .def(py::init<std::string, Trades::TradeType, double, int>())
      .def_readwrite("ticker", &Trades::Trade_Message::ticker)
      .def_readwrite("trade_type", &Trades::Trade_Message::trade_type)
      .def_readwrite("price", &Trades::Trade_Message::price)
      .def_readwrite("quantity", &Trades::Trade_Message::quantity);

  py::class_<MarketDataframe>(m, "MarketDataframe")
      .def(py::init<>())
      // This exposes the map. pybind11/stl.h automatically converts
      // std::unordered_map <-> dict and std::deque <-> list.
      .def_readwrite("sf_ll", &MarketDataframe::sf_ll);
}
