#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib> // for system()

struct DayData {
    std::string date;
    double close;
};

// Function to read CSV
std::vector<DayData> readCSV(const std::string& filename) {
    std::vector<DayData> data;
    std::ifstream file(filename);
    std::string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string date_str, price_str;
        getline(ss, date_str, ',');
        getline(ss, price_str, ',');
        DayData day;
        day.date = date_str;
        day.close = std::stod(price_str);
        data.push_back(day);
    }
    return data;
}

// Function to calculate moving average
double movingAverage(const std::vector<DayData>& data, int endIndex, int window) {
    if (endIndex + 1 < window) return 0; // not enough data
    double sum = 0;
    for (int i = endIndex - window + 1; i <= endIndex; ++i)
        sum += data[i].close;
    return sum / window;
}

int main() {
    // Parameters
    std::string filename = "stock_prices.csv";
    int shortWindow = 20; // 20-day MA
    int longWindow = 50;  // 50-day MA
    double cash = 10000;  // initial capital
    int shares = 0;

    // Read data
    std::vector<DayData> data = readCSV(filename);

    std::vector<std::string> signals(data.size(), "HOLD");
    std::vector<double> portfolio(data.size(), 0);

    // Backtest
    for (size_t i = 0; i < data.size(); ++i) {
        double shortMA = movingAverage(data, i, shortWindow);
        double longMA = movingAverage(data, i, longWindow);

        // Generate signals
        if (shortMA > longMA && shares == 0) {
            // Buy signal
            shares = cash / data[i].close;
            cash = 0;
            signals[i] = "BUY";
        } else if (shortMA < longMA && shares > 0) {
            // Sell signal
            cash = shares * data[i].close;
            shares = 0;
            signals[i] = "SELL";
        }

        // Portfolio value
        portfolio[i] = cash + shares * data[i].close;
    }

    // Export results
    std::string output_csv = "portfolio_results.csv";
    std::ofstream out(output_csv);
    out << "Date,Close,Signal,Portfolio\n";
    for (size_t i = 0; i < data.size(); ++i)
        out << data[i].date << "," << data[i].close << "," << signals[i] << "," << portfolio[i] << "\n";
    out.close();

    std::cout << "Backtest complete! Results saved in " << output_csv << "\n";

    // Automatically call Python visualization
    std::cout << "Launching Python visualization...\n";
    system("python plot_results.py"); // Make sure plot_results.py exists in the same folder

    return 0;
}
