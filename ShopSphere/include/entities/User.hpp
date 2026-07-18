// stringstream processes data stored in a string, whereas ifstream reads from a file and ofstream writes to a file.

#pragma once
#include <string>

using namespace std;

class User{
private:
    int id; 
    string name;    
    string email;   
    string password;    
    string role;

public:

    User();

    User(int id, string name, string email, string password, string role);

    void setId(int id);

    void setName(string name);

    void setEmail(string email);

    void setPassword(string password);

    void setRole(string role);

    int getId() const;

    string getName() const;

    string getEmail() const;

    string getPassword() const;

    string getRole() const;

    string serialize() const;

    static User deserialize(string record);

    void display() const;
};
