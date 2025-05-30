#include <cstdint>
#include <string>

enum class Side { BUY, SELL };
enum class Type { MARKET, LIMIT };
enum class Action { NEW, MODIFY, CANCEL };

struct Order {
    uint64_t timestamp;
    int order_id;
    std::string instrument;
    Side side;
    Type type;
    int quantity;
    float price;
    Action action;
};