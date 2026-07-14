#include "entities/OrderItem.hpp"
#include <sstream>
#include <iomanip>
using namespace std;

OrderItem::OrderItem(int orderId, int productId, string productName, double price, int quantity){
    this->orderId = orderId;
    this->productId = productId;
    this->productName = productName;
    this->price = price;
    this->quantity = quantity;
}

int OrderItem::getOrderId() const{
    return orderId;
}

int OrderItem::getProductId() const{
    return productId;
}

string OrderItem::getProductName() const{
    return productName;
}

double OrderItem::getPrice() const{
    return price;
}

int OrderItem::getQuantity() const{
    return quantity;
}

string OrderItem::serialize() const{
    stringstream ss;

    ss << orderId << "|"
       << productId << "|"
       << productName << "|"
       << fixed << setprecision(2) << price << "|"
       << quantity;

    return ss.str();
}

OrderItem OrderItem::deserialize(string record){
    stringstream ss(record);

    string orderId;
    string productId;
    string productName;
    string price;
    string quantity;

    getline(ss, orderId, '|');
    getline(ss, productId, '|');
    getline(ss, productName, '|');
    getline(ss, price, '|');
    getline(ss, quantity);

    return OrderItem(stoi(orderId), stoi(productId), productName, stod(price), stoi(quantity));
}