#include "Indicators.h"

double EMA(double price, double prevEMA, int window) {
    double alpha = 2.0 / (window + 1);
    return alpha * price + (1 - alpha) * prevEMA;
}

double RSI(const std::vector<double>& closes, int endIndex, int period) {
    if (endIndex < period) return 50;
    double gain = 0, loss = 0;
    for (int i = endIndex - period + 1; i <= endIndex; ++i) {
        double change = closes[i] - closes[i - 1];
        if (change > 0) gain += change;
        else loss -= change;
    }
    double avgGain = gain / period;
    double avgLoss = loss / period;
    if (avgLoss == 0) return 100;
    double rs = avgGain / avgLoss;
    return 100 - (100 / (1 + rs));
}
