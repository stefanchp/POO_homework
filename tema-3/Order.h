#ifndef ORDER_H
#define ORDER_H

#include <string>       
#include <vector>       
#include <iostream>     
#include <numeric>      
#include <memory>       
#include <chrono>       
#include <stdexcept>    

class OrderBuilder;

struct OrderItem {
    std::string productId_;
    int quantity_;
    double unitPrice_;

    OrderItem(std::string id, int qty, double price)
        : productId_(std::move(id)), quantity_(qty), unitPrice_(price) {}

    double getItemTotal() const 
    {
        return quantity_ * unitPrice_;
    }

    void display() const 
    {
        std::cout << "    - Product: " << productId_
                  << ", Qty: " << quantity_
                  << ", Unit Price: $" << unitPrice_
                  << ", Item Total: $" << getItemTotal() << '\n';
    }

    double getPrice() const { return getItemTotal(); }
};

class Order {
private:
    std::string orderId_;
    std::string customerId_;
    std::vector<OrderItem> items_;
    double totalAmount_ = 0.0;
    std::string shippingAddress_;
    bool isGiftWrapped_ = false;

    Order(std::string orderId, std::string customerId, std::vector<OrderItem> items,
          std::string shippingAddress, bool giftWrapped)
        : orderId_(std::move(orderId)), customerId_(std::move(customerId)),
          items_(std::move(items)), shippingAddress_(std::move(shippingAddress)),
          isGiftWrapped_(giftWrapped) {
        for(const auto& item : items_) 
            totalAmount_ += item.getItemTotal();
        
    }

public:
    friend class OrderBuilder;

    void displayOrder() const {
        std::cout << "\n--- Order Details ---\n";
        std::cout << "Order ID: " << orderId_ << '\n';
        std::cout << "Customer ID: " << customerId_ << '\n';
        std::cout << "Shipping Address: " << shippingAddress_ << '\n';
        std::cout << "Items:\n";
        for (const auto& item : items_) 
            item.display();
        std::cout << "Gift Wrapped: " << (isGiftWrapped_ ? "Yes" : "No") << '\n';
        std::cout << "Total Amount: $" << totalAmount_ << '\n';
        std::cout << "---------------------\n";
    }

    double getTotalAmount() const { return totalAmount_; }
    const std::vector<OrderItem>& getItems() const { return items_; }
    const std::string& getOrderId() const { return orderId_; }
};

class OrderBuilder {
private:
    // id-uri unice cu chrono
    std::string orderId_ = "ORD" +
                           std::to_string(
                               std::chrono::system_clock::now()
                                   .time_since_epoch()
                                   .count() % 1000000         
                           );
    std::string customerId_;
    std::vector<OrderItem> items_;
    std::string shippingAddress_;
    bool isGiftWrapped_ = false;

public:
    OrderBuilder(std::string customerId) : customerId_(std::move(customerId)) {}

    OrderBuilder& withOrderId(std::string id) 
    {
        orderId_ = std::move(id);
        return *this;
    }

    OrderBuilder& addItem(const std::string& productId, int quantity, double unitPrice) 
    {
        if (quantity <= 0 || unitPrice < 0)
            throw std::invalid_argument("Quantity must be positive and unit price non-negative.");
        items_.emplace_back(productId, quantity, unitPrice);
        return *this;
    }

    OrderBuilder& setShippingAddress(const std::string& address) 
    {
        shippingAddress_ = address;
        return *this;
    }

    OrderBuilder& setGiftWrapped(bool wrapped) 
    {
        isGiftWrapped_ = wrapped;
        return *this;
    }

    Order build() 
    {
        if (customerId_.empty())
            throw std::runtime_error("Customer ID is mandatory for an order.");
        if (items_.empty())
            throw std::runtime_error("Order must contain at least one item.");
        if (shippingAddress_.empty())
            throw std::runtime_error("Shipping address is mandatory.");
        return Order(orderId_, customerId_, items_, shippingAddress_, isGiftWrapped_);
    }
};

#endif