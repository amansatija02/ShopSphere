#pragma once
#include <vector>
#include "entities/User.hpp"

using namespace std;
class AccountService{
private:
    vector<User> users;
    User currentUser;
    bool loggedIn;
    void loadUsers();
    bool emailExists(string email);
    int nextUserId();

public:
    AccountService();
    bool registerUser(User user);
    bool login(string email, string password);
    void logout();
    bool isLoggedIn() const;
    User getCurrentUser() const;
};
