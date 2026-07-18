#include "input/InputGuard.hpp"
#include <cctype>
using namespace std;

bool InputGuard::isEmpty(string value){
    return value.empty();
}

bool InputGuard::validEmail(string email){
    if(email.empty()) return false;

    int atCount = 0;
    int atPos = -1;
    int dotPos = -1;

    for(int i = 0; i < email.length(); i++){
        if(email[i] == ' ')  return false;

        if(email[i] == '@'){
            atCount++;
            atPos = i;
        }

        if(email[i] == '.') dotPos = i;
    }

    if(atCount != 1) return false;
    if(atPos == 0) return false;
    if(dotPos < atPos + 2) return false;
    if(dotPos == email.length() - 1) return false;
    return true;
}

bool InputGuard::validPassword(string password){

    if(password.length() < 8) return false;

    bool upper = false;
    bool lower = false;
    bool digit = false;

    for(int i = 0; i < password.length(); i++){
        if(isupper(password[i])) upper = true;
        else if(islower(password[i])) lower = true;
        else if(isdigit(password[i])) digit = true;
    }
    return upper && lower && digit;
}