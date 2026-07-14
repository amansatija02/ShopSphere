#pragma once
#include <string>
using namespace std;

class OrderItem{
private:
    int orderId;
    int productId;
    string productName;
    double price;
    int quantity;

public:
    OrderItem(int orderId = 0, int productId = 0, string productName = "", double price = 0, int quantity = 0);

    int getOrderId() const;
    int getProductId() const;
    string getProductName() const;
    double getPrice() const;
    int getQuantity() const;
    string serialize() const;
    static OrderItem deserialize(string record);
};

