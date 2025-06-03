#include "../src/csv_parser.hpp"
#include "../src/csv_writer.hpp"
#include "../src/order_book.hpp"
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>

// Fonction pour générer un ordre aléatoire
Order generateRandomOrder(int id, uint64_t timestamp, const std::string& instrument) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> side_dist(0, 1);
    static std::uniform_int_distribution<> type_dist(0, 1);
    static std::uniform_int_distribution<> action_dist(0, 2);
    static std::uniform_int_distribution<> quantity_dist(1, 1000);
    static std::uniform_real_distribution<> price_dist(10.0, 1000.0);
    
    Order order;
    order.timestamp = timestamp;
    order.order_id = id;
    order.instrument = instrument;
    order.side = side_dist(gen) == 0 ? Side::BUY : Side::SELL;
    order.type = type_dist(gen) == 0 ? Type::MARKET : Type::LIMIT;
    order.quantity = quantity_dist(gen);
    order.price = static_cast<float>(price_dist(gen));
    order.action = static_cast<Action>(action_dist(gen));
    
    return order;
}

// Fonction pour générer un grand nombre d'ordres pour le benchmark
std::vector<Order> generateOrdersForBenchmark(int count) {
    std::vector<Order> orders;
    uint64_t timestamp = 1617278400000000000;
    
    for (int i = 0; i < count; i++) {
        // Générer des ordres pour différents instruments
        std::string instrument = "INSTR" + std::to_string(i % 10);
        orders.push_back(generateRandomOrder(i, timestamp, instrument));
        timestamp += 100000; // Incrémenter le timestamp
    }
    
    return orders;
}

int main() {
    std::cout << "=== Matching Engine Benchmark ===" << std::endl;
    
    // Nombre d'ordres à tester
    const std::vector<int> orderCounts = {100, 1000, 10000, 100000};
    
    for (int count : orderCounts) {
        std::cout << "\nTesting with " << count << " orders:" << std::endl;
        
        // Générer les ordres
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<Order> orders = generateOrdersForBenchmark(count);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> generateTime = end - start;
        std::cout << "  - Generation time: " << generateTime.count() << " ms" << std::endl;
        
        // Traiter les ordres
        start = std::chrono::high_resolution_clock::now();
        
        std::unordered_map<std::string, OrderBook> orderBooks;
        
        for (const auto& order : orders) {
            // Créer l'OrderBook pour cet instrument s'il n'existe pas déjà
            if (orderBooks.find(order.instrument) == orderBooks.end()) {
                orderBooks.emplace(order.instrument, OrderBook(order.instrument));
            }
            
            // Traiter l'ordre selon son action
            switch (order.action) {
                case Action::NEW:
                    orderBooks[order.instrument].addOrder(order);
                    break;
                    
                case Action::MODIFY:
                    orderBooks[order.instrument].modifyOrder(order);
                    break;
                    
                case Action::CANCEL:
                    orderBooks[order.instrument].cancelOrder(order.order_id);
                    break;
            }
        }
        
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> processTime = end - start;
        
        std::cout << "  - Processing time: " << processTime.count() << " ms" << std::endl;
        std::cout << "  - Average time per order: " << processTime.count() / count << " ms" << std::endl;
        
        // Mesurer la mémoire utilisée (cette méthode est approximative)
        size_t memoryUsage = 0;
        for (const auto& [instrument, book] : orderBooks) {
            // Estimer la taille des ordres dans les maps d'achat et de vente
            memoryUsage += book.getBuySide().size() * sizeof(std::pair<double, std::list<Order>>);
            memoryUsage += book.getSellSide().size() * sizeof(std::pair<double, std::list<Order>>);
        }
        
        std::cout << "  - Estimated memory usage: " << memoryUsage / 1024 << " KB" << std::endl;
    }
    
    return 0;
}
