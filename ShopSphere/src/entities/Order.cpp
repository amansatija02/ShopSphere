#include "entities/Order.hpp"
#include <sstream>
#include <iomanip>
using namespace std;

Order::Order(int orderId, int userId, double total, string status){
    this->orderId = orderId;
    this->userId = userId;
    this->total = total;
    this->status = status;
}

int Order::getOrderId() const{
    return orderId;
}

int Order::getUserId() const{
    return userId;
}

double Order::getTotal() const{
    return total;
}

string Order::getStatus() const{
    return status;
}

void Order::setStatus(string status){
    this->status = status;
}

string Order::serialize() const{
    stringstream ss;

    ss << orderId << "|"
       << userId << "|"
       << fixed << setprecision(2) << total << "|"
       << status;

    return ss.str();
}

Order Order::deserialize(string record){
    stringstream ss(record);

    string orderId;
    string userId;
    string total;
    string status;

    getline(ss, orderId, '|');
    getline(ss, userId, '|');
    getline(ss, total, '|');
    getline(ss, status);

    return Order(stoi(orderId), stoi(userId), stod(total), status);
}
