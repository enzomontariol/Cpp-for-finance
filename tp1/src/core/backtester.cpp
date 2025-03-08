#include <iostream>
#include "core/backtester.h"

Backtester::Backtester(): file_("marketdata.csv") {
    std::cout << "Init backtester" << std::endl;
}

Backtester::~Backtester() {
    std::cout << "Destruction backtester" << std::endl;
}

void Backtester::init() {
    std::string line;
    while (std::getline(file_, line)) {
        std::cout << line << std::endl;

        MarketData marketdata;

        int location = line.find(',');
        marketdata.timestamp = stoi(line.substr(0,location));
        location = stoi(line.substr(location + 1, line.length()));

        marketdata.instrument = line.substr(0,location);
        location = stoi(line.substr(location + 1, line.length()));;


        marketdata.price = strtof(line.substr(0,location));
        location = stoi(line.substr(location + 1, line.length()));



        marketdata.volume = 1.;
        marketdatas.push_back(marketdata);
    }
}

void Backtester::run() {
    std::cout << "run backtester" << std::endl;
}
