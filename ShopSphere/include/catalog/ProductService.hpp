#pragma once

#include <vector>
#include <unordered_map>

#include "entities/Product.hpp"

using namespace std;

class ProductService{
private:

    vector<Product> products;
    unordered_map<int, int> productIndex;

    void loadProducts();
    void rebuildIndex();

    bool saveAllProducts();

    int nextProductId();

public:

    ProductService();

    bool addProduct(Product product);

    bool updateProduct(Product product);

    bool deleteProduct(int id);

    Product searchById(int id);

    vector<Product> searchByCategory(string category);

    vector<Product> getProducts() const;

    void sortByPrice();

    void sortByStock();

    void displayProducts() const;

    int totalStock() const;

    double inventoryValue() const;
};