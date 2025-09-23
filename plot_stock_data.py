import pandas as pd
import matplotlib.pyplot as plt

# Ask user for CSV filename
filename = input("Enter the CSV filename (e.g., MSFT_historical_data.csv): ")

# Read CSV: skip the first 3 rows (extra headers)
data = pd.read_csv(filename, skiprows=3, names=['Date', 'Close', 'High', 'Low', 'Open', 'Volume'])

# Convert Date to datetime
data['Date'] = pd.to_datetime(data['Date'], format='%Y-%m-%d')

# Ask user for short-term and long-term MA periods
short_ma = int(input("Enter short-term MA period (e.g., 20): "))
long_ma = int(input("Enter long-term MA period (e.g., 50): "))

# Calculate moving averages
data['MA_Short'] = data['Close'].rolling(window=short_ma).mean()
data['MA_Long'] = data['Close'].rolling(window=long_ma).mean()

# Plot Close price and MAs
plt.figure(figsize=(14,6))
plt.plot(data['Date'], data['Close'], label='Close Price', color='blue')
plt.plot(data['Date'], data['MA_Short'], label=f'MA{short_ma}', color='green')
plt.plot(data['Date'], data['MA_Long'], label=f'MA{long_ma}', color='red')

plt.title("Stock Closing Price with Moving Averages")
plt.xlabel("Date")
plt.ylabel("Price")
plt.grid(True)
plt.legend()
plt.show()
