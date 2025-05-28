#include <iostream>
#include <vector>
#include <memory> // For std::unique_ptr

#include "ProductAttribute.h"
#include "InventoryManager.h"
#include "Order.h"
#include "PaymentStrategy.h"
#include "EcommerceUtils.h"

int main() 
{
    std::cout << "--- E-commerce System Start ---\n";

    std::cout << "\n--- ProductAttribute<T> ---\n";
    ProductAttribute<std::string> productName("Product Name", "SuperWidget");
    ProductAttribute<double> productPrice("Price", 29.99);
    ProductAttribute<int> productStock("Stock", 50);
    ProductAttribute<Dimensions> shippingDims("Shipping Dimensions", {10, 5, 2});

    std::cout << productName << '\n';
    std::cout << productPrice << '\n';
    std::cout << productStock << '\n';
    std::cout << shippingDims << '\n';

    productStock.updateValue(-5); // rejected
    productStock.updateValue(45); // accepted
    shippingDims.updateValue({0,0,0}); // rejected
    std::cout << "Updated Stock: " << productStock.getValue() << '\n';
    std::cout << "Updated Dimensions: " << shippingDims.getValue() << '\n';


    std::cout << "\n--- InventoryManager (Singleton) ---\n";
    InventoryManager& invManager = InventoryManager::getInstance();
    invManager.addProduct("PROD001", 5); // adaugam inca 5
    invManager.addProduct("PROD004", 15);
    invManager.printInventory();
    std::cout << "Stock of PROD001: " << invManager.getStock("PROD001") << '\n';

    std::cout << "\n--- OrderBuilder ---\n";
    Order order1 = OrderBuilder("CUST123")
                       .addItem("PROD001", 2, 29.99) // se stie pretul cand comanzi
                       .addItem("PROD004", 1, 15.50)
                       .setShippingAddress("123 Main St, Anytown, USA")
                       .setGiftWrapped(true)
                       .build();
    order1.displayOrder();
    
    // Reducem stocul in InventoryManager
    for (const auto& item : order1.getItems()) 
        invManager.reduceStock(item.productId_, item.quantity_);
    invManager.printInventory();

    try {
        Order orderFail = OrderBuilder("CUST456").build(); // Fails: no items, no address
    } catch (const std::runtime_error& e) {
        std::cerr << "OrderBuilder Error: " << e.what() << '\n';
    }

    std::cout << "\n--- PaymentStrategy ---" << '\n';
    PaymentProcessor processor;

    // platim cu cardul 
    processor.setPaymentStrategy(std::make_unique<CreditCardPayment>("1234-5678-9012-3456", "12/25", "123"));
    bool paymentStatus1 = processor.processPayment(order1);
    std::cout << "Order 1 Payment Status: " << (paymentStatus1 ? "SUCCESSFUL" : "FAILED") << '\n';

    // acum vrem sa platim cu PayPal
    Order order2 = OrderBuilder("CUST789")
                       .addItem("PROD002", 1, 99.99)
                       .setShippingAddress("456 Oak Ave, Otherville, USA")
                       .build();
    order2.displayOrder();

    
    processor.setPaymentStrategy(std::make_unique<PayPalPayment>("customer@example.com"));
    bool paymentStatus2 = processor.processPayment(order2);
    std::cout << "Order 2 Payment Status: " << (paymentStatus2 ? "SUCCESSFUL" : "FAILED") << '\n';

    std::cout << "\n--- Free Template Function (calculateTotalPrice) ---" << '\n';
    double order1TotalCalculated = calculateTotalPrice(order1.getItems());
    std::cout << "Calculated total for order1 items: $" << order1TotalCalculated << " (Actual: $" << order1.getTotalAmount() << ")\n";
    
    std::vector<OrderItem> customItems;
    customItems.emplace_back("CUSTOM01", 1, 10.0);
    customItems.emplace_back("CUSTOM02", 3, 5.0);
    double customTotal = calculateTotalPrice(customItems);
    std::cout << "Calculated total for custom item list: $" << customTotal << '\n';


    std::cout << "\n--- E-commerce System End ---\n";
    return 0;
}