#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib> // for system()

struct DayData
{
	std::string date;
	double close;
};

// Read CSV downloaded from yfinance (skip extra header rows)
std::vector<DayData> readCSV(const std::string &filename)
{
	std::vector<DayData> data;
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Error opening file " << filename << "\n";
		return data;
	}

	std::string line;
	// Skip first 3 header rows (Price, Ticker, Date,,,,,)
	for (int i = 0; i < 3; ++i)
		getline(file, line);

	while (getline(file, line))
	{
		if (line.empty())
			continue;
		std::stringstream ss(line);
		std::string date_str, close_str;
		getline(ss, date_str, ',');	 // Date
		getline(ss, close_str, ','); // Close
		try
		{
			DayData day;
			day.date = date_str;
			day.close = std::stod(close_str);
			data.push_back(day);
		}
		catch (...)
		{
			continue; // skip invalid rows
		}
	}
	return data;
}

// Calculate moving average
double movingAverage(const std::vector<DayData> &data, int endIndex, int window)
{
	if (endIndex + 1 < window)
		return 0; // not enough data
	double sum = 0;
	for (int i = endIndex - window + 1; i <= endIndex; ++i)
		sum += data[i].close;
	return sum / window;
}

int main()
{
	// --- Parameters ---
	std::cout << "Enter the CSV file name: ";
	std::string filename;
	std::getline(std::cin, filename);
	//  = "stock_prices.csv"; // CSV from yfinance
	int shortWindow, longWindow;
	double cash = 10000; // initial capital
	int shares = 0;

	std::cout << "Enter short-term MA period (e.g., 20): ";
	std::cin >> shortWindow;
	std::cout << "Enter long-term MA period (e.g., 50): ";
	std::cin >> longWindow;

	// --- Read Data ---
	std::vector<DayData> data = readCSV(filename);
	if (data.empty())
	{
		std::cerr << "No data found. Check CSV formatting!\n";
		return 1;
	}

	std::vector<std::string> signals(data.size(), "HOLD");
	std::vector<double> portfolio(data.size(), 0);

	// --- Backtest ---
	for (size_t i = 0; i < data.size(); ++i)
	{
		double shortMA = movingAverage(data, i, shortWindow);
		double longMA = movingAverage(data, i, longWindow);

		if (shortMA > longMA && shares == 0)
		{
			// Buy
			shares = cash / data[i].close;
			cash = 0;
			signals[i] = "BUY";
		}
		else if (shortMA < longMA && shares > 0)
		{
			// Sell
			cash = shares * data[i].close;
			shares = 0;
			signals[i] = "SELL";
		}

		// Portfolio value
		portfolio[i] = cash + shares * data[i].close;
	}

	// --- Write CSV ---
	std::string output_csv = "portfolio_results.csv";
	std::ofstream out(output_csv);
	out << "Date,Close,Signal,Portfolio\n";
	for (size_t i = 0; i < data.size(); ++i)
		out << data[i].date << "," << data[i].close << "," << signals[i] << "," << portfolio[i] << "\n";
	out.close();

	std::cout << "Backtest complete! Results saved in " << output_csv << "\n";

	// --- Call Python visualization ---
	std::cout << "Launching Python visualization...\n";
	// After writing portfolio_results.csv, call Python with MA periods
	std::string python_cmd = "python plot_results.py " + std::to_string(shortWindow) + " " + std::to_string(longWindow);
	system(python_cmd.c_str());

	return 0;
}
