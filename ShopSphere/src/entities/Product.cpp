#include "entities/Product.hpp"
#include <sstream>
#include <iomanip>
using namespace std;

Product::Product(int id, string name, string category, double price, int stock){
    this->id = id;
    this->name = name;
    this->category = category;
    this->price = price;
    this->stock = stock;
}

void Product::setId(int id){
    this->id = id;
}

int Product::getId() const{
    return id;
}

void Product::setName(string name){
    this->name = name;
}

string Product::getName() const{
    return name;
}

void Product::setCategory(string category){
    this->category = category;
}

string Product::getCategory() const{
    return category;
}

void Product::setPrice(double price){
    this->price = price;
}

double Product::getPrice() const{
    return price;
}

void Product::setStock(int stock){
    this->stock = stock;
}

int Product::getStock() const{
    return stock;
}

string Product::serialize() const{
    stringstream ss;

    ss << id << '|'
       << name << '|'
       << category << '|'
       << price << '|'
       << stock;

    return ss.str();
}

Product Product::deserialize(string record){
    stringstream ss(record);
    string id;
    string name;
    string category;
    string price;
    string stock;

    getline(ss, id, '|');
    getline(ss, name, '|');
    getline(ss, category, '|');
    getline(ss, price, '|');
    getline(ss, stock);

    return Product(stoi(id), name, category, stod(price), stoi(stock));
}