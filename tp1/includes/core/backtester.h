#ifndef TP1_BACKTESTER_H
#define TP1_BACKTESTER_H

#include <fstream>
#include <vector>

struct MarketData {
    long int timestamp;
    std::string instrument;
    double price;
    double volume;
};

class Backtester {
public:
    Backtester();
    ~Backtester();

    void init();
    void run();
protected:
private:
    std::fstream file_;
    std::vector<MarketData> marketdatas;
};

#endif //TP1_BACKTESTER_H
