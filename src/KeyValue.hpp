#pragma once

#include <string>
#include <variant>
#include "ValueVisiter.hpp"

using namespace std;
using multiType = std::variant<string, double, int, bool>;

const char commentSign = ';';
const char commentSign2 = '#';

string& trimR(string& str);
string& trimL(string& str);
string& trim(string& str);

struct KeyValue{
    string key;
    multiType value;
    bool isComment;

    KeyValue(string& aKey, const variant<string, double, int, bool>& aValue);

    string toString();
    string getKey();
    void setKey(string& newKey);
    bool operator<(const KeyValue& other);
};
