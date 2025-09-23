import sys
import pandas as pd
import matplotlib.pyplot as plt

# Get short and long MA from command-line arguments
if len(sys.argv) < 3:
    print("Usage: python plot_results.py <short_MA> <long_MA>")
    sys.exit(1)

short_ma = int(sys.argv[1])
long_ma = int(sys.argv[2])

# Read backtest results
data = pd.read_csv("portfolio_results.csv")

# Convert Date to datetime
data['Date'] = pd.to_datetime(data['Date'], format='%Y-%m-%d')

# Calculate moving averages
data['MA_Short'] = data['Close'].rolling(window=short_ma).mean()
data['MA_Long'] = data['Close'].rolling(window=long_ma).mean()

# Plot Close price with Buy/Sell signals and MAs
plt.figure(figsize=(14,6))
plt.plot(data['Date'], data['Close'], label='Close Price', color='blue')
plt.plot(data['Date'], data['MA_Short'], label=f'MA{short_ma}', color='green', linestyle='--')
plt.plot(data['Date'], data['MA_Long'], label=f'MA{long_ma}', color='red', linestyle='--')

# Plot Buy/Sell markers
buy_signals = data[data['Signal'] == 'BUY']
sell_signals = data[data['Signal'] == 'SELL']
plt.scatter(buy_signals['Date'], buy_signals['Close'], marker='^', color='green', label='Buy', s=100)
plt.scatter(sell_signals['Date'], sell_signals['Close'], marker='v', color='red', label='Sell', s=100)

plt.title("Backtest: Stock Price with MAs and Buy/Sell Signals")
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
