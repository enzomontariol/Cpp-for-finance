#include <fstream>
#include <string>
#include <iostream>

#include "core/backtester.h"

int main() {
    Backtester bt;

    bt.init();
    bt.run();
    return 0;
}
