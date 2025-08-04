#include <Python.h>
#include "../utils/trade_message.h"
#include "../utils/market_dataframe.h"



namespace PyBinder {
  // Forward declarations
  Trades::Trade_Message getTradeMessageFromPython(
        const std::string& module_name,
        const std::string& function_name,
        const MarketDataframe& market_data
        );
  PyObject* convertToPythonMarketData(const MarketDataframe& market_data);
  Trades::Trade_Message parsePythonTradeMessage(PyObject* pValue);
  
  Trades::Trade_Message getTradeMessageFromPython(
        const std::string& module_name,
        const std::string& function_name,
        const MarketDataframe& market_data
        ) {
        PyObject* pName = PyUnicode_DecodeFSDefault(module_name.c_str());
        if (!pName) {
            PyErr_Print();
            return {"", static_cast<Trades::TradeType>(0), 0.0, 0};
        }

        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (!pModule) {
            PyErr_Print();
            return {"", static_cast<Trades::TradeType>(0), 0.0, 0};
        }

        PyObject* pFunc = PyObject_GetAttrString(pModule, function_name.c_str());
        if (!pFunc || !PyCallable_Check(pFunc)) {
            if (PyErr_Occurred()) PyErr_Print();
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            return {"", static_cast<Trades::TradeType>(0), 0.0, 0};
        }

        PyObject* pArgs = PyTuple_New(1);
        if (!pArgs) {
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            return {"", static_cast<Trades::TradeType>(0), 0.0, 0};
        }

        PyObject* pMarketDict = convertToPythonMarketData(market_data);
        if (!pMarketDict) {
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            Py_DECREF(pArgs);
            return {"", static_cast<Trades::TradeType>(0), 0.0, 0};
        }

        PyTuple_SetItem(pArgs, 0, pMarketDict);

        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
        
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        Py_DECREF(pModule);

        if (pValue == nullptr) {
            PyErr_Print();
            return {"", static_cast<Trades::TradeType>(0), 0.0, 0};
        }

        Trades::Trade_Message trade_message = parsePythonTradeMessage(pValue);
        Py_DECREF(pValue);

        return trade_message;
    }

  // Helper function to convert MarketDataframe to a nested Python dictionary.
  // This function creates a Python object graph from C++ data.
  PyObject* convertToPythonMarketData(const MarketDataframe& market_data) {
      PyObject* pMarketDict = PyDict_New();
      if (!pMarketDict) return nullptr;

      for (const auto& pair : market_data.sf_ll) {
          // TODO check that this conversion is ok
          // Explicitly cast the unsigned size() to a signed Py_ssize_t
          PyObject* pStockList = PyList_New(static_cast<Py_ssize_t>(pair.second.size()));
          if (!pStockList) {
              Py_XDECREF(pMarketDict);
              return nullptr;
          }

          int i = 0;
          for (const auto& sf : pair.second) {
              PyObject* pStockFrameDict = PyDict_New();
              if (!pStockFrameDict) {
                  Py_XDECREF(pStockList);
                  Py_XDECREF(pMarketDict);
                  return nullptr;
              }

              PyDict_SetItemString(pStockFrameDict, "ticker", PyUnicode_FromString(std::string(sf.ticker).c_str()));
              PyDict_SetItemString(pStockFrameDict, "curr_price", PyFloat_FromDouble(sf.curr_price));
              PyDict_SetItemString(pStockFrameDict, "vol_traded", PyLong_FromLong(sf.vol_traded));
              
              PyList_SetItem(pStockList, i, pStockFrameDict);
              i++;
          }
          PyDict_SetItemString(pMarketDict, pair.first.c_str(), pStockList);
      }
      return pMarketDict;
  }

  // Helper function to parse the returned Python dictionary into a TradeMessage struct.
  // This function extracts data from a Python object into our C++ struct.
  Trades::Trade_Message parsePythonTradeMessage(PyObject* pValue) {
      Trades::Trade_Message trade_message = {"", static_cast<Trades::TradeType>(0), 0.0, 0};

      if (pValue && PyDict_Check(pValue)) {
          PyObject* pTicker = PyDict_GetItemString(pValue, "ticker");
          if (pTicker && PyUnicode_Check(pTicker)) {
              trade_message.ticker = PyUnicode_AsUTF8(pTicker);
          }

          PyObject* pTradeType = PyDict_GetItemString(pValue, "trade_type");
          if (pTradeType && PyLong_Check(pTradeType)) {
              trade_message.trade_type = static_cast<Trades::TradeType>(PyLong_AsLong(pTradeType));
          }

          PyObject* pPrice = PyDict_GetItemString(pValue, "price");
          if (pPrice && PyFloat_Check(pPrice)) {
              trade_message.price = PyFloat_AsDouble(pPrice);
          }

          PyObject* pQuantity = PyDict_GetItemString(pValue, "quantity");
          if (pQuantity && PyLong_Check(pQuantity)) {
              trade_message.quantity = static_cast<int>(PyLong_AsLong(pQuantity));
          }
      }
      return trade_message;
  }
}
