#include <iostream>
#include <fstream>
#include <sstream>
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

        std::stringstream ss(line);
        MarketData marketdata;
        std::string token;
        
        // lire le timestamp
        if (!std::getline(ss, token, ',')) continue;
        marketdata.timestamp = std::stoi(token); //conversion du token en int
        
        if (!std::getline(ss, marketdata.instrument, ',')) continue; //le contenu est directement du bon type, pas besoin de passer par un token et une conversion

        if (!std::getline(ss, token, ',')) continue;
        marketdata.price = std::stof(token); //conversion du token en float

        if (!std::getline(ss, token, ',')) continue;
        marketdata.volume = std::stof(token); //conversion du token en float

        marketdatas.push_back(marketdata);
    }
}

void Backtester::run() {
    std::cout << "run backtester" << std::endl;
}
