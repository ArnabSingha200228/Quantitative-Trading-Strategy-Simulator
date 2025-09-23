import yfinance as yf

# Ask user for inputs
ticker = input("Enter stock symbol (e.g., AAPL): ").upper()
start_date = input("Enter start date (YYYY-MM-DD): ")
end_date = input("Enter end date (YYYY-MM-DD): ")
period = input("Enter data interval (daily, weekly, monthly) [d/w/m]: ").lower()

# Map user input to yfinance interval
interval_map = {'d': '1d', 'w': '1wk', 'm': '1mo'}
interval = interval_map.get(period, '1d')  # default to daily

# Download the data
data = yf.download(ticker, start=start_date, end=end_date, interval=interval)

# Save to CSV
filename = f"{ticker}_historical_data.csv"
data.to_csv(filename)

print(f"\nData downloaded for {ticker} from {start_date} to {end_date} ({interval})")
print(f"Saved to {filename}")
