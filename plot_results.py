import pandas as pd
import matplotlib.pyplot as plt

# Read backtest results
data = pd.read_csv("portfolio_results.csv")

# Convert Date to datetime
data['Date'] = pd.to_datetime(data['Date'], format='%Y-%m-%d')

# Plot Close price with Buy/Sell markers
plt.figure(figsize=(14,6))
plt.plot(data['Date'], data['Close'], label='Close Price', color='blue')

buy_signals = data[data['Signal'] == 'BUY']
sell_signals = data[data['Signal'] == 'SELL']

plt.scatter(buy_signals['Date'], buy_signals['Close'], marker='^', color='green', label='Buy', s=100)
plt.scatter(sell_signals['Date'], sell_signals['Close'], marker='v', color='red', label='Sell', s=100)

plt.title("Backtest: Stock Price with Buy/Sell Signals")
plt.xlabel("Date")
plt.ylabel("Price")
plt.legend()
plt.grid(True)
plt.show()

# Plot Portfolio value over time
plt.figure(figsize=(14,6))
plt.plot(data['Date'], data['Portfolio'], label='Portfolio Value', color='purple')
plt.title("Portfolio Value Over Time")
plt.xlabel("Date")
plt.ylabel("Portfolio Value")
plt.legend()
plt.grid(True)
plt.show()
