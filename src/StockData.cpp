#include "StockData.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool StockData::loadCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << "\n";
        return false;
    }

    std::string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string date_str, open, high, low, close_str, adj_close, volume;
        getline(ss, date_str, ',');  
        getline(ss, open, ',');
        getline(ss, high, ',');
        getline(ss, low, ',');
        getline(ss, close_str, ','); 
        getline(ss, adj_close, ',');
        getline(ss, volume, ',');
        try {
            DayData day;
            day.date = date_str;
            day.close = std::stod(close_str);
            data.push_back(day);
        } catch (...) {
            continue;
        }
    }
    return !data.empty();
}
