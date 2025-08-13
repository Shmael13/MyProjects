import matplotlib.pyplot as plt
import trader_bindings as sim

def display_market_data_plot(market_data: sim.MarketDataframe):
    tickers = iter(market_data.sf_ll)
    
    for ticker in tickers:
        stock_frames = market_data.sf_ll[ticker]
        prices = [frame.curr_price for frame in stock_frames]
        timesteps = list(range(len(prices)))

        plt.title(f"{ticker} Stock")
        plt.xlabel("Timestep")
        plt.ylabel("Price")
        plt.plot(timesteps, prices)
        plt.show()

