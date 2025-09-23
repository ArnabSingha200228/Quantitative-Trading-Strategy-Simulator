#include <iostream>
#include "StockData.h"
#include "Backtest.h"
#include "Utils.h"
#include <string>
#include <fstream>
#include <sys/stat.h>   // mkdir on Linux/macOS
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>     // _mkdir on Windows
#endif

// Function to create directory if not exists
void createDirectory(const std::string &dirname) {
#ifdef _WIN32
    _mkdir(dirname.c_str());   // Windows
#else
    mkdir(dirname.c_str(), 0777); // Linux/macOS
#endif
}

int main()
{
    std::string symbol, start_date, end_date, period;
    int shortEMA, longEMA;

    std::cout << "Enter stock symbol: ";
    std::cin >> symbol;
    std::cout << "Enter period: ";
    std::cin >> period;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> start_date;
    std::cout << "Enter end date (YYYY-MM-DD, optional, press enter for today): ";
    std::getline(std::cin >> std::ws, end_date);
    if (end_date.empty())
        end_date = "";

    std::cout << "Enter short-term EMA period: ";
    std::cin >> shortEMA;
    std::cout << "Enter long-term EMA period: ";
    std::cin >> longEMA;

    // Step 1: Download data via Python
    callPythonDownload(symbol, period, start_date, end_date);

    // Step 2: Load stock data
    StockData stockData;
    std::string csv_file = "data/" + symbol + "_historical.csv";
    if (!stockData.loadCSV(csv_file))
    {
        std::cerr << "Failed to load stock data!\n";
        return 1;
    }

    // Step 3: Run backtest
    BacktestResult result = runBacktest(stockData, shortEMA, longEMA);

    // Step 4: Create output folder
    std::string outputDir = "output";
    createDirectory(outputDir);

    // Step 5: Save results CSV inside output folder
    std::string outputFile = outputDir + "/portfolio_results.csv";
    std::ofstream out(outputFile);
    out << "Date,Close,Signal,Portfolio,EMA_Short,EMA_Long,RSI\n";
    for (size_t i = 0; i < stockData.data.size(); ++i)
        out << stockData.data[i].date << "," << stockData.data[i].close << "," << result.signals[i] << ","
            << result.portfolio[i] << "," << result.emaShort[i] << "," << result.emaLong[i] << "," << result.rsi[i] << "\n";
    out.close();

    std::cout << "Results saved in: " << outputFile << std::endl;

    // Step 6: Call Python plot
    callPythonPlot(shortEMA, longEMA);

    return 0;
}
