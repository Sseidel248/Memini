#include "KeyValue.hpp"

string& trimL(string& str) {
    size_t pos = str.find_first_not_of(" \n\r");
    if (pos != string::npos) {
        str.erase(0, pos);
    } else {
        str.clear();
    }
    return str;
}

string& trimR(string& str) {
    size_t endPos = str.find_last_not_of(" \n\r");
    if (endPos != string::npos) {
        str = str.substr(0, endPos + 1);
    } else {
        str.clear();
    }
    return str;
}

string& trim(string& str) {
    return trimL(trimR(str));
}

KeyValue::KeyValue(string& aKey, const variant<string, double, int, bool>& aValue)
: value(aValue){
    setKey(aKey);
    isComment = key.starts_with(commentSign) || key.starts_with(commentSign2);
}

string KeyValue::toString(){
    string strValue = visit(ValueVisiter(), value);
    if (isComment) {
        if (key.starts_with(commentSign) || key.starts_with(commentSign2)) {
            return key + "=" + strValue + "\n";
        }
        return getKey() + "=" + strValue + "\n";
    } else {
        return key + "=" + strValue + "\n";
    }
}

string KeyValue::getKey() {
    if (isComment) {
        return commentSign + key;
    } else {
        return key;
    } 
}

void KeyValue::setKey(string& newKey) {
    key = trim(newKey);
}

bool KeyValue::operator<(const KeyValue& other) {
    return key < other.key;
}
