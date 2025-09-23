#include <iostream>
#include "StockData.h"
#include "Backtest.h"
#include "Utils.h"
#include <string>
#include <fstream>

int main() {
    std::string symbol, start_date, end_date;
    int shortEMA, longEMA;

    std::cout << "Enter stock symbol: ";
    std::cin >> symbol;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> start_date;
    std::cout << "Enter end date (YYYY-MM-DD, optional, press enter for today): ";
    std::getline(std::cin >> std::ws, end_date);
    if (end_date.empty()) end_date = ""; 

    std::cout << "Enter short-term EMA period: ";
    std::cin >> shortEMA;
    std::cout << "Enter long-term EMA period: ";
    std::cin >> longEMA;

    callPythonDownload(symbol, start_date, end_date);

    StockData stockData;
    std::string csv_file = "data/" + symbol + "_historical.csv";
    if (!stockData.loadCSV(csv_file)) {
        std::cerr << "Failed to load stock data!\n";
        return 1;
    }

    BacktestResult result = runBacktest(stockData, shortEMA, longEMA);

    // Save results CSV
    std::ofstream out("portfolio_results.csv");
    out << "Date,Close,Signal,Portfolio,EMA_Short,EMA_Long,RSI\n";
    for (size_t i = 0; i < stockData.data.size(); ++i)
        out << stockData.data[i].date << "," << stockData.data[i].close << "," << result.signals[i] << ","
            << result.portfolio[i] << "," << result.emaShort[i] << "," << result.emaLong[i] << "," << result.rsi[i] << "\n";
    out.close();

    callPythonPlot(shortEMA, longEMA);

    return 0;
}
