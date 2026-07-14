#include "orders/OrderService.hpp"
#include "storage/DataStore.hpp"
#include "core/AppConfig.hpp"
#include "basket/CartService.hpp"
#include "catalog/ProductService.hpp"
#include "audit/AuditService.hpp"
#include <iostream>

using namespace std;

OrderService::OrderService(){
    loadOrders();
    loadOrderItems();
}

void OrderService::loadOrders(){
    orders.clear();

    DataStore store;

    vector<string> records = store.readRecords(Config::ORDER_RECORD);

    for(size_t i = 0; i < records.size(); i++){
        orders.push_back(Order::deserialize(records[i]));

        if(orders.back().getStatus() == "Pending"){
            pendingOrders.push(orders.back().getOrderId());
        }
    }
}

void OrderService::loadOrderItems(){
    orderItems.clear();

    DataStore store;

    vector<string> records = store.readRecords(Config::ORDER_ITEM_RECORD);

    for(size_t i = 0; i < records.size(); i++){
        orderItems.push_back(OrderItem::deserialize(records[i]));
    }
}

bool OrderService::saveOrders(){
    DataStore store;

    store.clearRecords(Config::ORDER_RECORD);

    for(size_t i = 0; i < orders.size(); i++){
        store.saveRecord(Config::ORDER_RECORD, orders[i].serialize());
    }

    return true;
}

bool OrderService::saveOrderItems(){
    DataStore store;

    store.clearRecords(Config::ORDER_ITEM_RECORD);

    for(size_t i = 0; i < orderItems.size(); i++){
        store.saveRecord(Config::ORDER_ITEM_RECORD, orderItems[i].serialize());
    }

    return true;
}

int OrderService::nextOrderId(){
    if(orders.empty()) return 1;

    int maxId = orders[0].getOrderId();

    for(size_t i = 1; i < orders.size(); i++){
        if(orders[i].getOrderId() > maxId){
            maxId = orders[i].getOrderId();
        }
    }

    return maxId + 1;
}

bool OrderService::checkout(int userId){
    CartService cartService;
    ProductService productService;
    AuditService audit;

    vector<CartItem> cart = cartService.getCartItems(userId);

    if(cart.empty()) return false;

    int orderId = nextOrderId();

    double total = 0;

    for(size_t i = 0; i < cart.size(); i++){
        Product product = productService.searchById(cart[i].getProductId());

        if(product.getId() == 0) return false;

        if(cart[i].getQuantity() > product.getStock()) return false;

        total += product.getPrice() * cart[i].getQuantity();
    }

    orders.push_back(Order(orderId, userId, total, "Pending"));

    for(size_t i = 0; i < cart.size(); i++){
        Product product = productService.searchById(cart[i].getProductId());

        orderItems.push_back(
            OrderItem(orderId, product.getId(), product.getName(), product.getPrice(), cart[i].getQuantity())
        );

        product.setStock(product.getStock() - cart[i].getQuantity());

        productService.updateProduct(product);
    }

    saveOrders();
    saveOrderItems();

    pendingOrders.push(orderId);

    cartService.clearCart(userId);

    audit.info("Order Placed");

    return true;
}

void OrderService::processOrders(){
    loadOrders();

    while(!pendingOrders.empty()){
        int orderId = pendingOrders.front();
        pendingOrders.pop();

        for(size_t i = 0; i < orders.size(); i++){
            if(orders[i].getOrderId() == orderId){
                if(orders[i].getStatus() == "Pending"){
                    orders[i].setStatus("Completed");
                }
                break;
            }
        }
    }

    saveOrders();

    AuditService audit;
    audit.info("Pending Orders Processed");
}

void OrderService::displayOrders(int userId){
    loadOrders();

    bool found = false;
    cout << "\n------ Orders ------\n";

    for(size_t i = 0; i < orders.size(); i++){
        if(orders[i].getUserId() != userId) continue;

        cout << "\nOrder ID : "  << orders[i].getOrderId();
        cout << "\nTotal : " << orders[i].getTotal();
        cout << "\nStatus : " << orders[i].getStatus();
        cout << "\n";

        found = true;
    }

    if(!found) cout << "No Orders Found.\n";
}

void OrderService::displayOrderDetails(int orderId){
    loadOrderItems();
    bool found = false;

    cout << "\n------ Order Details ------\n";

    for(size_t i = 0; i < orderItems.size(); i++){
        if(orderItems[i].getOrderId() != orderId) continue;

        cout << "\nProduct : " << orderItems[i].getProductName();
        cout << "\nPrice : " << orderItems[i].getPrice();
        cout << "\nQuantity : " << orderItems[i].getQuantity();
        cout << "\nSubtotal : " << orderItems[i].getPrice() * orderItems[i].getQuantity();
        cout << "\n";

        found = true;
    }

    if(!found) cout << "No Products Found.\n";
}

bool OrderService::cancelOrder(int userId, int orderId){
    loadOrders();

    for(size_t i = 0; i < orders.size(); i++){
        if(orders[i].getOrderId() != orderId) continue;
        if(orders[i].getUserId() != userId) return false;
        if(orders[i].getStatus() != "Pending") return false;

        orders[i].setStatus("Cancelled");

        saveOrders();

        AuditService audit;
        audit.info("Order Cancelled");

        return true;
    }

    return false;
}
