#include "storage/DataStore.hpp"
#include <fstream>

using namespace std;

bool DataStore::saveRecord(string fileName, string record){
    ofstream file(fileName, ios::app);

    if(!file.is_open()) return false;

    file << record << endl;

    file.close();

    return true;
}

vector<string> DataStore::readRecords(string fileName){
    vector<string> records;

    ifstream file(fileName);

    if(!file.is_open())
        return records;

    string line;

    while(getline(file, line)) {
        if(line.empty()) continue;

        records.push_back(line);
    }

    file.close();

    return records;
}

bool DataStore::clearRecords(string fileName){
    ofstream file(fileName);

    if(!file.is_open()) return false;

    file.close();

    return true;
}