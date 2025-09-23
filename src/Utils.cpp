#include "Utils.h"
#include <cstdlib>
#include <iostream>

void callPythonDownload(const std::string &symbol, const std::string &period, const std::string &start_date, const std::string &end_date)
{
	std::string cmd = "python data_downloader.py " + symbol + " " + period + " " + start_date + " " + end_date;
	std::cout << "Downloading stock data...\n";
	system(cmd.c_str());
}

void callPythonPlot(int shortEMA, int longEMA)
{
	std::string cmd = "python plot_results.py " + std::to_string(shortEMA) + " " + std::to_string(longEMA);
	std::cout << "Plotting results...\n";
	system(cmd.c_str());
}
