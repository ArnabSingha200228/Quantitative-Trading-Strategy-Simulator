#pragma once
#include <vector>

double EMA(double price, double prevEMA, int window);
double RSI(const std::vector<double>& closes, int endIndex, int period);
