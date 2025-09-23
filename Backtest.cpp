#include "Backtest.h"
#include "Indicators.h"
#include <vector>
#include <string>

BacktestResult runBacktest(const StockData& stockData, int shortEMA, int longEMA, double initialCash) {
    BacktestResult result;
    size_t n = stockData.data.size();

    std::vector<double> closes;
    for (auto &d : stockData.data) closes.push_back(d.close);

    result.emaShort.resize(n);
    result.emaLong.resize(n);
    result.rsi.resize(n);
    result.signals.resize(n, "HOLD");
    result.portfolio.resize(n, 0);

    result.emaShort[0] = closes[0];
    result.emaLong[0] = closes[0];
    result.rsi[0] = 50;

    double cash = initialCash;
    double shares = 0;

    for (size_t i = 1; i < n; ++i) {
        result.emaShort[i] = EMA(closes[i], result.emaShort[i-1], shortEMA);
        result.emaLong[i] = EMA(closes[i], result.emaLong[i-1], longEMA);
        result.rsi[i] = RSI(closes, i, 14);

        if (result.emaShort[i] > result.emaLong[i] && result.rsi[i] < 70 && shares == 0) {
            shares = cash / closes[i];
            cash = 0;
            result.signals[i] = "BUY";
        } else if (result.emaShort[i] < result.emaLong[i] && result.rsi[i] > 30 && shares > 0) {
            cash = shares * closes[i];
            shares = 0;
            result.signals[i] = "SELL";
        }
        result.portfolio[i] = cash + shares * closes[i];
    }

    return result;
}
