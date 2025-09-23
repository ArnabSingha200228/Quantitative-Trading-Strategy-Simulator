#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib> // system()

struct DayData
{
    std::string date;
    double close;
};

// Read CSV from yfinance
std::vector<DayData> readCSV(const std::string &filename)
{
    std::vector<DayData> data;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file\n";
        return data;
    }

    std::string line;
    // Skip first 3 header rows
    for (int i = 0; i < 3; ++i)
        getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string date_str, close_str;
        getline(ss, date_str, ',');  // Date
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
            continue;
        }
    }
    return data;
}

// EMA calculation
double EMA(double price, double prevEMA, int window)
{
    double alpha = 2.0 / (window + 1);
    return alpha * price + (1 - alpha) * prevEMA;
}

// RSI calculation
double RSI(const std::vector<double> &closes, int endIndex, int period)
{
    if (endIndex < period)
        return 50; // neutral

    double gain = 0, loss = 0;
    for (int i = endIndex - period + 1; i <= endIndex; ++i)
    {
        double change = closes[i] - closes[i - 1];
        if (change > 0)
            gain += change;
        else
            loss -= change;
    }
    double avgGain = gain / period;
    double avgLoss = loss / period;
    if (avgLoss == 0)
        return 100;
    double rs = avgGain / avgLoss;
    return 100 - (100 / (1 + rs));
}

int main()
{
    std::cout << "Enter the CSV file name: ";
    std::string filename;
    std::getline(std::cin, filename);
    int shortWindow, longWindow, rsiPeriod = 14;
    double cash = 10000;
    double shares = 0;

    std::cout << "Enter short-term EMA period: ";
    std::cin >> shortWindow;
    std::cout << "Enter long-term EMA period: ";
    std::cin >> longWindow;

    std::vector<DayData> data = readCSV(filename);
    if (data.empty())
    {
        std::cerr << "No data found!\n";
        return 1;
    }

    std::vector<double> closes;
    for (auto &d : data)
        closes.push_back(d.close);

    std::vector<double> emaShort(data.size(), 0);
    std::vector<double> emaLong(data.size(), 0);
    std::vector<double> rsi(data.size(), 50);

    // Compute EMAs
    emaShort[0] = closes[0];
    emaLong[0] = closes[0];
    for (size_t i = 1; i < data.size(); ++i)
    {
        emaShort[i] = EMA(closes[i], emaShort[i - 1], shortWindow);
        emaLong[i] = EMA(closes[i], emaLong[i - 1], longWindow);
        rsi[i] = RSI(closes, i, rsiPeriod);
    }

    // Backtest
    std::vector<std::string> signals(data.size(), "HOLD");
    std::vector<double> portfolio(data.size(), 0);

    for (size_t i = 0; i < data.size(); ++i)
    {
        if (emaShort[i] > emaLong[i] && rsi[i] < 70 && shares == 0)
        {
            // Buy
            shares = cash / closes[i];
            cash = 0;
            signals[i] = "BUY";
        }
        else if (emaShort[i] < emaLong[i] && rsi[i] > 30 && shares > 0)
        {
            // Sell
            cash = shares * closes[i];
            shares = 0;
            signals[i] = "SELL";
        }
        portfolio[i] = cash + shares * closes[i];
    }

    // Save CSV
    std::ofstream out("portfolio_results.csv");
    out << "Date,Close,Signal,Portfolio,EMA_Short,EMA_Long,RSI\n";
    for (size_t i = 0; i < data.size(); ++i)
        out << data[i].date << "," << closes[i] << "," << signals[i] << ","
            << portfolio[i] << "," << emaShort[i] << "," << emaLong[i] << "," << rsi[i] << "\n";
    out.close();

    std::cout << "Backtest complete! Results saved in portfolio_results.csv\n";

    // Call Python for visualization
    std::string python_cmd = "python plot_results_EMA_RSI.py " + std::to_string(shortWindow) + " " + std::to_string(longWindow);
    system(python_cmd.c_str());

    return 0;
}
