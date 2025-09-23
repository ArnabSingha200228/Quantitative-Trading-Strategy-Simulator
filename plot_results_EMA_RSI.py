import sys
import pandas as pd
import matplotlib.pyplot as plt

# Get EMA periods from command line
short_ma = int(sys.argv[1])
long_ma = int(sys.argv[2])

# Read CSV
data = pd.read_csv("portfolio_results.csv")
data['Date'] = pd.to_datetime(data['Date'], format='%Y-%m-%d')

# Buy/Sell markers
buy_signals = data[data['Signal'] == 'BUY']
sell_signals = data[data['Signal'] == 'SELL']

# Create a single figure with 3 subplots (Price+EMAs, Portfolio, RSI)
fig, axes = plt.subplots(3, 1, figsize=(14, 12), sharex=True)

# --- Subplot 1: Price + EMAs + Signals ---
axes[0].plot(data['Date'], data['Close'], label='Close', color='blue')
axes[0].plot(data['Date'], data['EMA_Short'], label=f'EMA{short_ma}', color='green', linestyle='--')
axes[0].plot(data['Date'], data['EMA_Long'], label=f'EMA{long_ma}', color='red', linestyle='--')
axes[0].scatter(buy_signals['Date'], buy_signals['Close'], marker='^', color='green', s=100, label='Buy')
axes[0].scatter(sell_signals['Date'], sell_signals['Close'], marker='v', color='red', s=100, label='Sell')
axes[0].set_ylabel("Price")
axes[0].set_title("Stock Price with EMA and Buy/Sell Signals")
axes[0].legend()
axes[0].grid(True)

# --- Subplot 2: Portfolio Value ---
axes[1].plot(data['Date'], data['Portfolio'], label='Portfolio', color='purple')
axes[1].set_ylabel("Portfolio Value")
axes[1].set_title("Portfolio Value Over Time")
axes[1].legend()
axes[1].grid(True)

# --- Subplot 3: RSI ---
axes[2].plot(data['Date'], data['RSI'], label='RSI', color='orange')
axes[2].axhline(70, color='red', linestyle='--')
axes[2].axhline(30, color='green', linestyle='--')
axes[2].set_xlabel("Date")
axes[2].set_ylabel("RSI")
axes[2].set_title("RSI Over Time")
axes[2].legend()
axes[2].grid(True)

plt.tight_layout()
plt.show()
