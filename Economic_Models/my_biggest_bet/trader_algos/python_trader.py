import sys
import numpy as np
import pandas as pd

def make_trade_decision(market_data):
    """
    Analyzes market data and returns a trade message.
    :param market_data: A dictionary where keys are market names and values are lists of stock data dictionaries.
    :return: A dictionary representing a trade message.
    """
    # This is a placeholder for your trading logic.
    # We'll just return a simple, static trade for demonstration.
    trade_message = {
        "ticker": "Apple",
        "trade_type": 1,  
        "price": 50.0,
        "quantity": 2
    }

    return trade_message
