#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <stdexcept> 

// CRTP Base for Singleton
template <typename Derived>
class BaseSingletonEcommerce {
protected:
    BaseSingletonEcommerce() = default;

public:
    BaseSingletonEcommerce(const BaseSingletonEcommerce&) = delete;
    BaseSingletonEcommerce& operator=(const BaseSingletonEcommerce&) = delete;
    BaseSingletonEcommerce(BaseSingletonEcommerce&&) = delete;
    BaseSingletonEcommerce& operator=(BaseSingletonEcommerce&&) = delete;

    static Derived& getInstance() 
    {
        static Derived instance;
        return instance;
    }
};

class InventoryManager : public BaseSingletonEcommerce<InventoryManager> {
private:
    std::map<std::string, int> productStock_; 
    // Key: ProductID, Value: Stock count

    InventoryManager() {
        productStock_["PROD001"] = 10;
        productStock_["PROD002"] = 5;
        productStock_["PROD003"] = 20;
        std::cout << "[InventoryManager] Initialized with random stock.\n";
    }

    friend class BaseSingletonEcommerce<InventoryManager>;

public:
    void addProduct(const std::string& productId, int initialStock) 
    {
        if (productStock_.count(productId)) 
        {
            productStock_[productId] += initialStock;
             std::cout << "[InventoryManager] Added " << initialStock << " to stock for " << productId 
                       << ". New stock: " << productStock_[productId] << "\n";
        } 
        else 
        {
            productStock_[productId] = initialStock;
            std::cout << "[InventoryManager] Added new product " << productId 
                      << " with stock: " << initialStock << '\n';
        }
    }

    bool checkStock(const std::string& productId, int quantity) const 
    {
        if (productStock_.count(productId)) 
            return productStock_.at(productId) >= quantity;
        return false;
    }

    bool reduceStock(const std::string& productId, int quantity) 
    {
        if (checkStock(productId, quantity)) 
        {
            productStock_[productId] -= quantity;
            std::cout << "[InventoryManager] Reduced stock for " << productId << " by " << quantity 
                      << ". Remaining stock: " << productStock_[productId] << '\n';
            return true;
        }
        std::cerr << "[InventoryManager] Failed to reduce stock for " << productId 
                  << ". Insufficient stock or product not found.\n";
        return false;
    }

    int getStock(const std::string& productId) const 
    {
        if (productStock_.count(productId))
            return productStock_.at(productId);
        return 0;
    }

    void printInventory() const 
    {
        std::cout << "\n--- Current Inventory ---\n";
        if (productStock_.empty()) 
        {
            std::cout << "Inventory is empty.\n";
            return;
        }
        for (const auto& pair : productStock_)
            std::cout << "Product ID: " << pair.first << ", Stock: " << pair.second << '\n';
        std::cout << "-------------------------\n";
    }
};

#endif