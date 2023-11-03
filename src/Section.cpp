#include "Section.hpp"

void Section::setSection(string& newSectName) {
    m_name = trim(newSectName);
}

Section::Section(string& name)
: m_ignoreComments(false){
    setSection(name);
}

Section::Section()
: m_name(""),
m_ignoreComments(false) {
}

void Section::addEntry(const string& line){
    if (line.empty()){
        return;
    }
    string key = "";
    string value = "";
    string part = "";
    // cut line in key and value
    size_t pos = line.find("=");
    if (pos != string::npos){
        key = line.substr(0, pos);
        value = line.substr(pos+1, line.length());
        addEntry(key, trim(value));
    }
}

void Section::addEntry(string &key, const multiType& value){
    if (key.empty()) {
        return;
    }
    m_content.push_back(KeyValue(key, value));
}

void Section::addEntryWithMultiValues(string& key, vector<multiType>& values) {  
    string result;
    for (multiType& aValue: values) {
        string value = visit(ValueVisiter(), aValue);
        if (!value.empty()) {
            result.append(value + ";");
        }
    }
    if (result.ends_with(";")) {
        result.erase(result.end() - 1);
    }
    m_content.push_back(KeyValue(key, result));
}

void Section::addEntries(vector<string> newEntries) {
    for (string entry:newEntries) {
        addEntry(entry);
    }
}

void Section::clear() {
    m_content.clear();
}

void Section::deleteEntry(const string& key) {
    int index = 0;
    bool found = false;
    for (KeyValue& pair : m_content) {      
        if (pair.key == key) {
            found = true;
            break;
        }
        index++;
    }
    if (found) {
        m_content.erase(m_content.begin() + index);
    }
}

vector<KeyValue> Section::getSection(){
    vector<KeyValue> copyContent(m_content);
    return copyContent;
}

multiType* Section::getValue(const string &key){
    for (KeyValue& pair: m_content){
        if (pair.key == key){
            return &pair.value;
        }
    }
    return nullptr;
}

void Section::setValue(string &key, const multiType& value){
    // check if the key exist
    for (KeyValue& pair: m_content){
        if (pair.key == key){
            pair.value = value;
            return;
        }
    }
    // Create the entry if not exist
    addEntry(key, value);
}

KeyValue* Section::getEntry(const string &key){
    for (KeyValue& pair: m_content){
        if (pair.key == key){
            return &pair;
        }
    }
    return nullptr;
}

string Section::getName(){
    return m_name;
}

void Section::setName(string& name) {
    if (trim(name) != "") {
        m_name = trim(name);
    }   
}

bool Section::existKey(const string& key) {
    for (const KeyValue& pair : m_content) {
        if (key == pair.key) {
            return true;
        }
    }
    return false;
}

bool Section::getIgnoreComments() {
    return m_ignoreComments;
}

void Section::setIgnoreComments(const bool value) {
    m_ignoreComments = value;
}

void Section::setAsComment(const string& key) {
    KeyValue* pair = getEntry(key);
    if (pair != nullptr) {
        pair->isComment = true;
    }
}

void Section::setAsNormal(const string& key) {
    KeyValue* pair = getEntry(key);
    if (pair != nullptr) {
        pair->isComment = false;
    }
}

string Section::toString(){
    string res = "["+m_name+"]\n";
    for (KeyValue pair: m_content){
        if (m_ignoreComments && (pair.isComment)) {
            continue;
        }
        res.append(pair.toString());        
    }
    return res;
}

void Section::renameKey(const string& oldKey, string& newKey) {
    if (!existKey(oldKey)) {
        return;
    }
    if (newKey.empty()) {
        return;
    }
    KeyValue* pair = getEntry(oldKey);
    pair->setKey(newKey);
}

size_t Section::getCount() {
    // count entries without comments
    if (m_ignoreComments) {
        int count = 0;
        for (KeyValue& pair : m_content) {
            if (!pair.isComment) {
                count++;
            }
        }
        return count;
    }
    return m_content.size();
}

void Section::sortKeys() {
    sort(m_content.begin(), m_content.end());
}

void Section::getKeys(vector<string>& allKeys) {
    for (KeyValue& pair :m_content) {
        allKeys.push_back(pair.getKey());
    }
}

bool Section::operator<(const Section& other) {
    return m_name < other.m_name;
}
