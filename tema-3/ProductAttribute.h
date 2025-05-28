#ifndef PRODUCT_ATTRIBUTE_H
#define PRODUCT_ATTRIBUTE_H

#include <string>
#include <iostream>
#include <type_traits>

struct Dimensions 
{
    double length = 0.0;
    double width = 0.0;
    double height = 0.0;

    friend std::ostream& operator<<(std::ostream& os, const Dimensions& d) 
    {
        os << d.length << "x" << d.width << "x" << d.height << " cm";
        return os;
    }
    bool operator==(const Dimensions& other) const 
    {
        return length == other.length && width == other.width && height == other.height;
    }
};


template <typename ValueType>
class ProductAttribute;

template <typename ValueType>
std::ostream& operator<<(std::ostream& os, const ProductAttribute<ValueType>& attr);

template <typename ValueType>
class ProductAttribute {
private:
    std::string name_;
    ValueType value_;

    // Implementare pentru tipuri int, double, long long
    template <typename VT = ValueType> 
    auto updateValue_impl(VT newValue, const std::string& attrName) ->
        typename std::enable_if_t<std::is_arithmetic<VT>::value, bool>
    {
        if (newValue < static_cast<VT>(0) && attrName == "Stock") 
        {
            std::cerr << "Warning: Stock cannot be negative for attribute '" << attrName << "'. Update rejected.\n";
            return false;
        }
        value_ = std::move(newValue);
        return true;
    }

    // Implementare pentru tipul Dimensions
    template <typename VT = ValueType>
    auto updateValue_impl(VT newValue, const std::string& attrName) ->
        typename std::enable_if_t<std::is_same<VT, Dimensions>::value, bool>
    {
        if (newValue.length <= 0 || newValue.width <= 0 || newValue.height <= 0) 
        {
             std::cerr << "Warning: Dimensions must be positive for attribute '" << attrName << "'. Update rejected.\n";
            return false;
        }
        value_ = std::move(newValue);
        return true;
    }

    /// pt orice alt tip de stock 
    template <typename VT = ValueType>
    auto updateValue_impl(VT newValue, const std::string&) ->
        typename std::enable_if_t<!std::is_arithmetic<VT>::value &&
                                  !std::is_same<VT, Dimensions>::value, bool>
    {
        value_ = std::move(newValue);
        return true;
    }

public:
    ProductAttribute(std::string name, ValueType initialValue)
        : name_(std::move(name)), value_(std::move(initialValue)) {}

    const std::string& getName() const 
    {
        return name_;
    }

    const ValueType& getValue() const 
    {
        return value_;
    }

    void setValue(ValueType newValue) 
    { 
        // setvalue fara validare
        value_ = std::move(newValue);
    }

    // se alege functia de update corecta
    bool updateValue(ValueType newValue) 
    {
        return updateValue_impl(std::move(newValue), name_);
    }

    friend std::ostream& operator<< <>(std::ostream& os, const ProductAttribute<ValueType>& attr);
};

template <typename ValueType>
std::ostream& operator<<(std::ostream& os, const ProductAttribute<ValueType>& attr) 
{
    os << attr.name_ << ": " << attr.value_;
    return os;
}

#endif