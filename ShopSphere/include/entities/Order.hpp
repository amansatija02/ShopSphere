#pragma once
#include <string>

using namespace std;

class Order{
private:
    int orderId;
    int userId;
    double total;
    string status;

public:

    Order(int orderId = 0, int userId = 0, double total = 0, string status = "Pending");

    int getOrderId() const;
    int getUserId() const;
    double getTotal() const;
    string getStatus() const;
    void setStatus(string status);
    string serialize() const;
    static Order deserialize(string record);
};
