#pragma once
#include <string>

void callPythonDownload(const std::string &symbol, const std::string &period, const std::string &start_date, const std::string &end_date);
void callPythonPlot(int shortEMA, int longEMA);
