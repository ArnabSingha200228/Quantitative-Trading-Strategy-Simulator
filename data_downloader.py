import sys
import yfinance as yf
import os
from datetime import datetime

if len(sys.argv) < 5:
    print("Usage: python download_data.py <symbol> <period> <start_date> <end_date(optional)>")
    sys.exit(1)

symbol = sys.argv[1]
period = sys.argv[2] #[d/w/m]
start_date = sys.argv[3]
end_date = sys.argv[4] if len(sys.argv) > 3 else datetime.today().strftime('%Y-%m-%d')

interval_map = {'d': '1d', 'w': '1wk', 'm': '1mo'}
interval = interval_map.get(period, '1d')  # default to daily

# Create data folder if it doesn't exist
os.makedirs("data", exist_ok=True)
file_path = f"data/{symbol}_historical.csv"

# Download stock data
df = yf.download(symbol, start=start_date, end=end_date, interval=interval)
df.to_csv(file_path)
print(file_path)
