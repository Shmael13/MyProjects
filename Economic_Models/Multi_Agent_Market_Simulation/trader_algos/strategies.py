# strategies.py
import random

# IMPORTANT: Import the C++ bindings module created
# The name 'trader_bindings' must match what's in CMakeLists.txt
import trader_bindings as sim
from collections import deque
import numpy as np
import tensorflow as tf
from tensorflow import keras
from sklearn.preprocessing import MinMaxScaler
import os

class LiveNNTrader:
    """
    A trader that trains a neural network live during the simulation.
    It collects data, and periodically retrains the model on new data.
    """
    def __init__(self):
        # --- Model and Scaler Configuration ---
        self.WINDOW_SIZE = 50
        self.TRAINING_INTERVAL = 100  # Train every 100 ticks
        self.INITIAL_TRAINING_POINTS = 200 # Must be > WINDOW_SIZE
        self.is_trained = False
        self.num_iterations = 0
        
        # --- Data Buffers ---
        # Use a deque for efficient appends and pops from the left
        self.prices = deque(maxlen=self.INITIAL_TRAINING_POINTS * 2) 

        # --- Initialize Model and Scaler ---
        self.model = self._build_model()
        self.scaler = MinMaxScaler(feature_range=(0, 1))

    def _build_model(self):
        """Creates and returns the Keras model architecture."""
        model = keras.Sequential()
        model.add(keras.layers.LSTM(units=50, return_sequences=True, input_shape=(self.WINDOW_SIZE, 1)))
        model.add(keras.layers.Dropout(0.2))
        model.add(keras.layers.LSTM(units=50))
        model.add(keras.layers.Dropout(0.2))
        model.add(keras.layers.Dense(units=1))
        model.compile(optimizer='adam', loss='mean_squared_error')
        return model

    def _train_model(self):
        """Trains the model on the data collected in the `prices` deque."""
        if len(self.prices) < self.INITIAL_TRAINING_POINTS:
            return False
            
        # Scale the data for training
        scaled_data = self.scaler.fit_transform(np.array(self.prices).reshape(-1, 1))

        X, y = [], []
        for i in range(self.WINDOW_SIZE, len(scaled_data)):
            X.append(scaled_data[i-self.WINDOW_SIZE:i, 0])
            y.append(scaled_data[i, 0])
        
        X = np.array(X).reshape(len(X), self.WINDOW_SIZE, 1)
        y = np.array(y)

        # Train for a small number of epochs to keep it fast
        self.model.fit(X, y, epochs=5, batch_size=32, verbose=0)
        return True

    def submit_trade(self, market_data: sim.MarketDataframe) -> sim.Trade_Message:
        self.num_iterations += 1
        ticker = next(iter(market_data.sf_ll))
        current_price = market_data.sf_ll[ticker][-1].curr_price
        
        # Add the latest price to our data buffer
        self.prices.append(current_price)
        
        # Initial training needs a sufficient amount of data
        if not self.is_trained and len(self.prices) >= self.INITIAL_TRAINING_POINTS:
            self.is_trained = self._train_model()
            if not self.is_trained: # Fallback if training failed
                return sim.Trade_Message("", sim.TradeType.MARKET_BUY, 0, 0)

        # Retrain periodically on new data
        if self.is_trained and (self.num_iterations >= self.TRAINING_INTERVAL):
            self._train_model()
            self.num_iterations = 0
            
        # Prediction logic
        if len(self.prices) < self.WINDOW_SIZE:
            # Not enough data for prediction yet
            return sim.Trade_Message("", sim.TradeType.MARKET_BUY, 0, 0)
        
        # if (self.is_trained) and len(self.prices) >= self.WINDOW_SIZE:
        # Prepare the input for the model
        input_data = np.array(list(self.prices)[-self.WINDOW_SIZE:]).reshape(-1, 1)
        scaled_input = self.scaler.transform(input_data)
        scaled_input = scaled_input.reshape(1, self.WINDOW_SIZE, 1)

        # Make a prediction
        predicted_scaled_price = self.model.predict(scaled_input, verbose=0)
        predicted_price = self.scaler.inverse_transform(predicted_scaled_price)[0][0]
        
         
        percentage_threshold = 0.001
        price_change_threshold = current_price * percentage_threshold

        # Decision logic
        
        if (predicted_price - current_price) > price_change_threshold:
            return sim.Trade_Message(ticker, sim.TradeType.MARKET_BUY, current_price, 1)
        elif (current_price - predicted_price) > price_change_threshold:
            return sim.Trade_Message(ticker, sim.TradeType.MARKET_SELL, current_price, 1)
        else:
            return sim.Trade_Message("", sim.TradeType.MARKET_BUY, 0, 0)


# Global variable to hold the single instance of LiveNNTrader
_live_nn_trader_instance = None

def get_live_nn_trade_message(market_data: sim.MarketDataframe) -> sim.Trade_Message:
    """
    Initializes a single instance of LiveNNTrader and uses it to generate
    a trade message based on the input MarketDataframe.

    Args:
        market_data (sim.MarketDataframe): The current market data.

    Returns:
        sim.Trade_Message: A trading message generated by the LiveNNTrader.
    """
    global _live_nn_trader_instance
    
    # Initialize the LiveNNTrader object only once
    if _live_nn_trader_instance is None:
        _live_nn_trader_instance = LiveNNTrader()
    
    # Use the existing (or newly created) instance to submit a trade
    return _live_nn_trader_instance.submit_trade(market_data)




# A simple moving average crossover strategy
def moving_avg_trader(market_data: sim.MarketDataframe) -> sim.Trade_Message:
    """
    A simple moving average crossover strategy.
    - If short-term MA crosses above long-term MA, BUY.
    - If short-term MA crosses below long-term MA, SELL.
    """
    # This trader only trades the first stock it sees
    # The market_data.sf_ll is a dict[str, list[StockFrame]]
    ticker = next(iter(market_data.sf_ll))
    
    stock_frames = market_data.sf_ll[ticker]
    
    if len(stock_frames) < 20:
        # Not enough data, do nothing
        return sim.Trade_Message("", sim.TradeType.MARKET_BUY, 0, 0)

    short_window = 5
    long_window = 20
    
    # Prices are the most recent N frames
    prices = [frame.curr_price for frame in stock_frames]
    
    # Calculate moving averages
    short_ma = sum(prices[-short_window:]) / short_window
    long_ma = sum(prices[-long_window:]) / long_window
    
    current_price = prices[-1]
    
    # Decision logic
    if short_ma > long_ma:
        # Bullish signal: buy 10 shares at market price
        return sim.Trade_Message(ticker, sim.TradeType.MARKET_BUY, current_price, 10)
    elif short_ma < long_ma:
        # Bearish signal: sell 10 shares at market price
        return sim.Trade_Message(ticker, sim.TradeType.MARKET_SELL, current_price, 10)
    else:
        # Indecisive, do nothing
        return sim.Trade_Message("", sim.TradeType.MARKET_BUY, 0, 0)


# A simple random trader for comparison
def random_py_trader(market_data: sim.MarketDataframe) -> sim.Trade_Message:
    ticker = next(iter(market_data.sf_ll))
    current_price = market_data.sf_ll[ticker][-1].curr_price

    if random.random() > 0.5:
        # Buy
        return sim.Trade_Message(ticker, sim.TradeType.MARKET_BUY, current_price, 5)
    else:
        # Sell
        return sim.Trade_Message(ticker, sim.TradeType.MARKET_SELL, current_price, 5)

