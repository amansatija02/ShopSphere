#pragma once
#include <vector>
#include <queue>
#include "entities/Order.hpp"
#include "entities/OrderItem.hpp"

using namespace std;

class OrderService{
private:
    vector<Order> orders;
    vector<OrderItem> orderItems;
    queue<int> pendingOrders;

    void loadOrders();
    void loadOrderItems();
    bool saveOrders();
    bool saveOrderItems();
    int nextOrderId();

public:

    OrderService();

    bool checkout(int userId);
    void processOrders();
    void displayOrders(int userId);
    void displayOrderDetails(int orderId);
    bool cancelOrder(int userId, int orderId);
};
