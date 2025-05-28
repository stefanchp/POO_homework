#ifndef ECOMMERCE_UTILS_H
#define ECOMMERCE_UTILS_H

#include <iostream>
#include <vector>
#include <numeric> 

// functii template pentru a calcula totalul comenzii
template <typename ItemContainer>
auto calculateTotalPrice(const ItemContainer& items) -> decltype(items.front().getPrice()) 
{
    using PriceType = decltype(items.front().getPrice());
    if (items.empty())
        return PriceType{0};
    PriceType total = PriceType{0};
    for (const auto& item : items) 
        total += item.getPrice();
    return total;
}

#endif 