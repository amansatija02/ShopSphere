#pragma once

#include <string>

using namespace std;

class Product{
private:
    int id;
    string name;
    string category;
    double price;
    int stock;

public:

    Product(int id = 0, string name = "", string category = "", double price = 0.0, int stock = 0);

    void setId(int id);
    int getId() const;

    void setName(string name);
    string getName() const;

    void setCategory(string category);
    string getCategory() const;

    void setPrice(double price);
    double getPrice() const;

    void setStock(int stock);
    int getStock() const;

    string serialize() const;
    static Product deserialize(string record);
};