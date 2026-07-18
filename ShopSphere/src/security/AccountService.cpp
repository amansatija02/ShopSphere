#include "security/AccountService.hpp"
#include "storage/DataStore.hpp"
#include "core/AppConfig.hpp"
#include "input/InputGuard.hpp"
#include "audit/AuditService.hpp"
#include <sstream>

using namespace std;

AccountService::AccountService(){
    loggedIn = false;
    loadUsers();
}

void AccountService::loadUsers(){
    DataStore store;
    vector<string> records;
    records = store.readRecords(Config::USER_DATA);

    users.clear();

    for(int i = 0; i < records.size(); i++){
        stringstream ss(records[i]);

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

        User user;

        user.setId(stoi(id));
        user.setName(name);
        user.setEmail(email);
        user.setPassword(password);
        user.setRole(role);

        users.push_back(user);
    }
}

bool AccountService::emailExists(string email){
    for(int i = 0; i < users.size(); i++) {
        if(users[i].getEmail() == email) return true;
    }
    return false;
}

bool AccountService::registerUser(User user){
    loadUsers();

    AuditService audit;
    InputGuard guard;

    if(guard.isEmpty(user.getName())){
        audit.failure("Name cannot be empty.");
        return false;
    }

    if(guard.isEmpty(user.getEmail())){
        audit.failure("Email cannot be empty.");
        return false;
    }

    if(guard.isEmpty(user.getPassword())){
        audit.failure("Password cannot be empty.");
        return false;
    }

    if(!guard.validEmail(user.getEmail())){
        audit.failure("Invalid email format.");
        return false;
    }

    if(!guard.validPassword(user.getPassword())){
        audit.failure("Password must contain at least 8 characters, one uppercase letter, one lowercase letter and one digit.");
        return false;
    }

    if(emailExists(user.getEmail())){
        audit.failure("Email already exists.");
        return false;
    }

    user.setId(nextUserId());

    DataStore store;

    if(store.saveRecord(Config::USER_DATA, user.serialize())){
        users.push_back(user);
        audit.info("User Registered Successfully.");
        return true;
    }

    audit.failure("Unable to save user.");
    return false;
}

bool AccountService::login(string email, string password){
    loadUsers();

    AuditService audit;

    for(int i = 0; i < users.size(); i++) {
        if(users[i].getEmail() == email &&  users[i].getPassword() == password) {
            currentUser = users[i];
            loggedIn = true;
            audit.info("Login Successful");
            return true;
        }
    }

    audit.failure("Invalid Email or Password");

    return false;
}

void AccountService::logout(){
    loggedIn = false;
    AuditService audit;
    audit.info("User Logged Out");
}

bool AccountService::isLoggedIn() const{
    return loggedIn;
}

User AccountService::getCurrentUser() const{
    return currentUser;
}

int AccountService::nextUserId(){
    loadUsers();

    if(users.empty()) return 1;

    int maxId = users[0].getId();

    for(size_t i = 1; i < users.size(); i++){
        if(users[i].getId() > maxId) maxId = users[i].getId();
    }

    return maxId + 1;
}