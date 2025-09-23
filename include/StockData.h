#pragma once
#include <vector>
#include <string>

struct DayData {
    std::string date;
    double close;
};

class StockData {
public:
    std::vector<DayData> data;

    bool loadCSV(const std::string& filename);
};
