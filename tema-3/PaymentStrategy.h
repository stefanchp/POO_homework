#ifndef PAYMENT_STRATEGY_H
#define PAYMENT_STRATEGY_H

#include <string>
#include <iostream>
#include "Order.h"

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount) = 0;
    virtual std::string getName() const = 0;
};

class CreditCardPayment : public PaymentStrategy {
private:
    std::string cardNumber_;
    std::string expiryDate_;
    std::string cvv_;

public:
    CreditCardPayment(std::string cardNum, std::string expiry, std::string cvv)
        : cardNumber_(std::move(cardNum)), expiryDate_(std::move(expiry)), cvv_(std::move(cvv)) {}

    bool pay(double amount) override {
        std::cout << "Processing credit card payment of $" << amount 
                  << " with card ending in " << cardNumber_.substr(cardNumber_.length() - 4) << '\n';
        if (amount > 1000.0) 
        { 
            //simulam o suma prea mare si o respingere
            std::cout << "Credit card payment DECLINED (amount too high for demo)." << '\n';
            return false;
        }
        std::cout << "Credit card payment APPROVED.\n";
        return true;
    }
    std::string getName() const override { return "Credit Card"; }
};

// Paypal
class PayPalPayment : public PaymentStrategy {
private:
    std::string email_;

public:
    PayPalPayment(std::string email) : email_(std::move(email)) {}

    bool pay(double amount) override 
    {
        std::cout << "Redirecting to PayPal for payment of $" << amount 
                  << " for account " << email_ << '\n';
        std::cout << "PayPal payment COMPLETED.\n";
        return true;
    }
    std::string getName() const override { return "PayPal"; }
};

class PaymentProcessor {
private:
    std::unique_ptr<PaymentStrategy> strategy_;

public:
    void setPaymentStrategy(std::unique_ptr<PaymentStrategy> strategy) 
    {
        strategy_ = std::move(strategy);
    }

    bool processPayment(const Order& order) 
    {
        if (!strategy_) 
        {
            std::cerr << "Error: No payment strategy set.\n";
            return false;
        }
        std::cout << "Attempting to process payment for Order ID: " << order.getOrderId() 
                  << " using " << strategy_->getName() << '\n';
        bool success = strategy_->pay(order.getTotalAmount());
        if (success) 
            std::cout << "Payment successful for Order ID: " << order.getOrderId() << '\n';
        else
            std::cout << "Payment FAILED for Order ID: " << order.getOrderId() << '\n';
        return success;
    }
};

#endif 