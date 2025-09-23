import pandas as pd
import matplotlib.pyplot as plt

# Ask user for CSV filename
filename = input("Enter the CSV filename (e.g., AAPL_historical_data.csv): ")

# Read the CSV
data = pd.read_csv(filename)

# Convert 'Date' to datetime
data['Date'] = pd.to_datetime(data['Date'])

# Plot Close price
plt.figure(figsize=(14,6))
plt.plot(data['Date'], data['Close'], label='Close Price', color='blue')

plt.title("Stock Closing Price Over Time")
plt.xlabel("Date")
plt.ylabel("Price")
plt.grid(True)
plt.legend()
plt.show()
