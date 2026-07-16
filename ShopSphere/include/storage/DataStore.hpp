#pragma once
#include <string>
#include <vector>
using namespace std;

class DataStore{
public:

    bool saveRecord(string fileName, string record);
    vector<string> readRecords(string fileName);
    bool clearRecords(string fileName);
};
