#include "entities/User.hpp"
#include <iostream>
#include <sstream>

using namespace std;

User::User(){
    id = 0;
    role = "Customer";
}

User::User(int id, string name, string email, string password, string role){
    this->id = id;
    this->name = name;
    this->email = email;
    this->password = password;
    this->role = role;
}

void User::setId(int id){
    this->id = id;
}

void User::setName(string name){
    this->name = name;
}

void User::setEmail(string email){
    this->email = email;
}

void User::setPassword(string password){
    this->password = password;
}

void User::setRole(string role){
    this->role = role;
}

int User::getId() const{
    return id;
}

string User::getName() const{
    return name;
}

string User::getEmail() const{
    return email;
}

string User::getPassword() const{
    return password;
}

string User::getRole() const{
    return role;
}

string User::serialize() const{
    stringstream ss;
    ss << id << "|";
    ss << name << "|";
    ss << email << "|";
    ss << password << "|";
    ss << role;

    return ss.str();
}

User User::deserialize(string record){
    stringstream ss(record);

    string id;
    string name;
    string email;
    string password;
    string role;

    getline(ss, id, '|');
    getline(ss, name, '|');
    getline(ss, email, '|');
    getline(ss, password, '|');
    getline(ss, role);

    return User(stoi(id), name, email, password, role);
}

void User::display() const{
    cout << "\nUser Details\n";
    cout << "------------\n";
    cout << "ID : " << id << endl;
    cout << "Name : " << name << endl;
    cout << "Email : " << email << endl;
    cout << "Role : " << role << endl;
}