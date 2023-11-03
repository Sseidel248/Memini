#include "Ini.hpp"

string lowercase(string& str) {
    string res = str;
    for (char& c : res) {
        c = tolower(c); // Wandele jeden Buchstaben in Kleinbuchstaben um
    }
    return res;
}

Ini::Ini(){
}

bool Ini::loadFromFile(const string& filepath) {
    m_filename = filepath;
    m_sections.clear();
    return load();
}

bool Ini::existSection(const string& sectName){
    for (Section& sect: m_sections){
        if (sect.getName() == sectName){
            return true;
        }
    }
    return false;
}

bool Ini::load(){
    m_sections.clear();
    //Open for read-only
    ifstream file(m_filename);
    if (file.is_open()){
        string line = "";
        string sectStr = "";
        while (getline(file, line)){
            //Set Section
            if (line.starts_with("[") && (line.ends_with("]"))){
                string name = line.substr(1, line.length() - 2);
                Section sect(name); 
                sectStr = sect.getName();             
                addSection(sect);  
            //Fill Section                  
            }else{
                Section* sect = nullptr;
                sect = getSection(sectStr);
                if (sect != nullptr){
                    sect->addEntry(line);
                }               
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool Ini::addSection(Section& newSect){
    if (existSection(newSect.getName())){
        return false;
    }
    newSect.setIgnoreComments(m_ignoreComments);
    m_sections.push_back(newSect);
    return true;
}

bool Ini::addSection(string sectName){
    Section newSect(sectName);
    return addSection(newSect);
}

Section* Ini::getSection(const string& sectName){
    //fix warning: Wreturn-local-addr 
    for (Section& sect: m_sections){
        if (sect.getName() == sectName){
            return &sect;
        }
    }
    return nullptr;
}

void Ini::setSection(string sectName, vector<string> newEntries) {
    Section* sect = getSection(sectName);
    if (sect != nullptr) {
        sect->clear();
        sect->addEntries(newEntries);
    } else {
        addSection(sectName);
        Section* newSect = getSection(sectName);
        newSect->addEntries(newEntries);
    }    
}

bool Ini::addEntry(string sectName, string key, string value){
    if (!existSection(sectName)){
        addSection(sectName);
    }
    Section* sect = getSection(sectName);
    int numKeys = sect->getCount();
    sect->addEntry(key, value);
    return sect->getCount() == numKeys + 1;
}

bool Ini::addEntry(string sectName, string key, vector<multiType> values) {
    if (!existSection(sectName)) {
        addSection(sectName);
    }
    Section* sect = getSection(sectName);
    int numKeys = sect->getCount();
    sect->addEntryWithMultiValues(key, values);
    return sect->getCount() == numKeys + 1;
}

vector<Section>& Ini::getSections() {
    return m_sections;
}

multiType* Ini::getValue(const string& sectName, const string& key){
    if (!existSection(sectName)){
        return nullptr;
    }
    Section* sect = getSection(sectName);
    return sect->getValue(key);
}

string Ini::getValueAsString(const string& sectName, const string& key, string defValue) {
    multiType* strValue = getValue(sectName, key);
    string visitValue = visit(ValueVisiter(), *strValue);
    if (visitValue.empty()) {
        return defValue;
    }
    return visitValue;
}

double Ini::getValueAsDouble(const string& sectName, const string& key, double defValue) {
    multiType* strValue = getValue(sectName, key);
    string visitValue = visit(ValueVisiter(), *strValue);
    double result;
    try {
        result = stod(visitValue);
    } catch (const std::invalid_argument e) {
        // Conversion failed
        result = defValue;
    } catch (const std::out_of_range e) {
        result = defValue;
    }
    return result;
}

int Ini::getValueAsInt(const string& sectName, const string& key, int defValue) {
    multiType* strValue = getValue(sectName, key);
    string visitValue = visit(ValueVisiter(), *strValue);
    int result;
    try {
        result = stoi(visitValue);
    } catch (const std::invalid_argument e) {
        // Conversion failed
        result = defValue;
    } catch (const std::out_of_range e) {
        result = defValue;
    }
    return result;
}

bool Ini::getValueAsBool(const string& sectName, const string& key, bool defValue) {
    multiType* strValue = getValue(sectName, key);
    string visitValue = visit(ValueVisiter(), *strValue);
    if (visitValue.empty()) {
        return defValue;
    }
    if ((lowercase(visitValue) != "true") && (lowercase(visitValue) != "false"))
        return defValue;

    if (lowercase(visitValue) == "true") {
        return true;
    } else{
        return false;
    }
}

vector<string> Ini::getValueAsList(const string& sectName, const string& key) {
    vector<string> resList;
    Section* sect = getSection(sectName);
    if (sect == nullptr) {
        return resList;
    }
    vector<string> keys;
    sect->getKeys(keys);
    for (string& aKey:keys) {
        if (aKey == key) {
            resList.push_back(visit(ValueVisiter(), *sect->getValue(key)));
        }       
    }
    return resList;
}

void Ini::setValue(string sectName, string key, multiType value){
    // Create if not exist    
    if (!existSection(sectName)){
        addSection(sectName);
    }       
    Section* sect = getSection(sectName);
    sect->setValue(key, value);
}

bool Ini::deleteEntry(const string& sectName, const string& key) {
    if (existSection(sectName)) {
        Section* sect = getSection(sectName);
        sect->deleteEntry(key);
        return true;
    }
    return false;
}

void Ini::sortSectionnames() {
    sort(m_sections.begin(), m_sections.end());
}

void Ini::sortSections() {
    sortSectionnames();
    for (Section& sect : m_sections) {
        sect.sortKeys();
    }
}

bool Ini::deleteEntryInAllSections(const string& key) {
    bool success = true;
    int numKeys = 0;
    for (Section& sect : m_sections) {
        sect.deleteEntry(key);
        success = success && sect.getCount() == 0;
    }
    return success;
}

void Ini::getSectionnames(vector<string>& result) {
    result.clear();
    if (!m_sections.empty()) {    
        for (Section& sect:m_sections) {
            result.push_back(sect.getName());
       }
    }  
}

void Ini::setEntryInAllSections(string key, const string& value) {
    for (Section& sect : m_sections) {
        sect.setValue(key, value);
    }
}

bool Ini::getIgnoreComments() {
    return m_ignoreComments;
}

void Ini::setIgnoreComments(const bool& value) {
    for (Section& sect : m_sections) {
        sect.setIgnoreComments(value);
    }
    m_ignoreComments = value;
}

void Ini::setAsComment(const string& sectName, const string& key) {
    Section* sect = getSection(sectName);
    if (sect != nullptr) {
        sect->setAsComment(key);
    }
}

void Ini::setAsNormal(const string& sectName, const string& key) {
    Section* sect = getSection(sectName);
    if (sect != nullptr) {
        sect->setAsNormal(key);
    }
}

bool Ini::renameKey(string sectName, const string& oldKey, string newKey) {
    if (!existSection(sectName)) {
        return false;
    }
    Section* sect = getSection(sectName);
    sect->renameKey(oldKey, newKey);
    return sect->existKey(newKey) && !sect->existKey(oldKey);
}

bool Ini::renameKeyInAllSections(const string& oldKey, string newKey) {
    bool success = true;
    int renameCount = 0;
    for (Section& sect : m_sections) {
        if (sect.existKey(oldKey)) {
            sect.renameKey(oldKey, newKey);
            success = success && sect.existKey(newKey) && !sect.existKey(oldKey);
            renameCount++;
        }
    }
    return success && renameCount > 0;
}

bool Ini::renameSection(const string& oldName, const string newName) {
    if (!existSection(oldName)) {
        return false;
    }
    Section* sect = getSection(oldName);
    //TODO: Neuen name setzen und anschließen prüfen!
    //sect->renameKey(oldKey, newKey);
    return sect->existKey(newKey) && !sect->existKey(oldKey);
}

string Ini::toString() {
    string res = "";
    for (Section sect:m_sections) {
        res.append(sect.toString());
        res.append("\n");
    }
    trimR(res);
    return res;
}

size_t Ini::getSectionCount() {
    return m_sections.size();
}

vector<string> Ini::getSectionKeys(const string& sectName) {
    vector<string> allKeys;
    Section* sect = getSection(sectName);
    if (sect != nullptr) {
        sect->getKeys(allKeys);
    }
    return allKeys;
}


