#include "catalog/ProductService.hpp"
#include "storage/DataStore.hpp"
#include "core/AppConfig.hpp"
#include "audit/AuditService.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

ProductService::ProductService(){
    loadProducts();
}

void ProductService::loadProducts(){
    products.clear();
    productIndex.clear();

    DataStore store;
    vector<string> records = store.readRecords(Config::PRODUCT_RECORD);

    for(size_t i = 0; i < records.size(); i++){
        stringstream ss(records[i]);

        string id, name, category, price, stock;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, category, '|');
        getline(ss, price, '|');
        getline(ss, stock);

        Product product(stoi(id), name, category, stod(price), stoi(stock));

        products.push_back(product);
        productIndex[product.getId()] = products.size() - 1;
    }
}

void ProductService::rebuildIndex(){
    productIndex.clear();

    for(size_t i = 0; i < products.size(); i++){
        productIndex[products[i].getId()] = i;
    }
}

int ProductService::nextProductId(){
    int maxId = 0;

    for(size_t i = 0; i < products.size(); i++){
        if(products[i].getId() > maxId)
            maxId = products[i].getId();
    }

    return maxId + 1;
}

bool ProductService::saveAllProducts(){
    DataStore store;

    store.clearRecords(Config::PRODUCT_RECORD);

    for(size_t i = 0; i < products.size(); i++){
        store.saveRecord(
            Config::PRODUCT_RECORD,
            products[i].serialize()
        );
    }

    rebuildIndex();

    return true;
}

bool ProductService::addProduct(Product product){
    loadProducts();

    product.setId(nextProductId());

    products.push_back(product);

    saveAllProducts();

    AuditService().info("Product Added");

    return true;
}

bool ProductService::updateProduct(Product product){
    loadProducts();

    if(productIndex.find(product.getId()) == productIndex.end()) return false;

    products[productIndex[product.getId()]] = product;

    saveAllProducts();

    AuditService().info("Product Updated");

    return true;
}

bool ProductService::deleteProduct(int id){
    loadProducts();

    if(productIndex.find(id) == productIndex.end()) return false;

    products.erase(products.begin() + productIndex[id]);

    saveAllProducts();

    AuditService().info("Product Deleted");

    return true;
}

Product ProductService::searchById(int id){
    if(productIndex.find(id) != productIndex.end()) return products[productIndex[id]];

    return Product();
}

vector<Product> ProductService::searchByCategory(string category){
    vector<Product> result;

    for(size_t i = 0; i < products.size(); i++){
        if(products[i].getCategory() == category)
            result.push_back(products[i]);
    }

    return result;
}

vector<Product> ProductService::getProducts() const{
    return products;
}

void ProductService::sortByPrice(){
    sort(products.begin(), products.end(), [](const Product& a, const Product& b){
            return a.getPrice() < b.getPrice();
        });

    rebuildIndex();
}

void ProductService::sortByStock(){
    sort(products.begin(), products.end(), [](const Product& a, const Product& b){
            return a.getStock() > b.getStock();
        });

    rebuildIndex();
}

void ProductService::displayProducts() const{
    if(products.empty()){
        cout << "No Products Available.\n";
        return;
    }

    cout << "\n----- Product List -----\n";

    for(size_t i = 0; i < products.size(); i++){
        cout << "\nID : " << products[i].getId();
        cout << "\nName : " << products[i].getName();
        cout << "\nCategory : " << products[i].getCategory();

        cout << "\nPrice : "
             << fixed
             << setprecision(2)
             << products[i].getPrice()
             << defaultfloat;

        cout << "\nStock : " << products[i].getStock();
        cout << "\n";
    }
}

int ProductService::totalStock() const {
    int total = 0;

    for(size_t i = 0; i < products.size(); i++)
        total += products[i].getStock();

    return total;
}

double ProductService::inventoryValue() const{
    double total = 0;

    for(size_t i = 0; i < products.size(); i++)
        total += products[i].getPrice() * products[i].getStock();

    return total;
}