#pragma once
#include <vector>
#include <string>
#include "StockData.h"

struct BacktestResult {
    std::vector<std::string> signals;
    std::vector<double> portfolio;
    std::vector<double> emaShort;
    std::vector<double> emaLong;
    std::vector<double> rsi;
};

BacktestResult runBacktest(const StockData& stockData, int shortEMA, int longEMA, double initialCash=10000);
