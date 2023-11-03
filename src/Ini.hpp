#pragma once

#include <fstream>
#include <string>
#include <variant>
#include <vector>
#include <algorithm>
#include <stdarg.h>
#include <cctype>
#include "Section.hpp"

using namespace std;

class Ini{
private:
    string m_filename = "";
    vector<Section> m_sections;
    bool m_ignoreComments = true;

    bool load();
public:
    Ini();
    bool loadFromFile(const string& filepath);
    //TODO: bool saveToFile(const string& filepath);
    bool existSection(const string& sectName);
    bool addSection(Section& newSect);
    bool addSection(string sectName);
    Section* getSection(const string& sectName);
    vector<Section>& getSections();
    void setSection(string sectName, vector<string> newEntries);
    bool addEntry(string sectName, string key, string value);
    bool addEntry(string sectName, string key, vector<multiType> values);
    bool deleteEntry(const string& sectName, const string& key);
    bool deleteEntryInAllSections(const string& key);
    //TODO: bool deleteSection(const string& sectName);
    void setEntryInAllSections(string key, const string& value);
    multiType* getValue(const string& sectName, const string& key);
    string getValueAsString(const string& sectName, const string& key, string defValue = "");
    double getValueAsDouble(const string& sectName, const string& key, double defValue = 0.0);
    int getValueAsInt(const string& sectName, const string& key, int defValue = 0);
    bool getValueAsBool(const string& sectName, const string& key, bool defValue = false);
    vector<string> getValueAsList(const string& sectName, const string& key);
    void setValue(string sectName, string key, multiType value);
    void sortSectionnames();
    void sortSections();
    void getSectionnames(vector<string>& result);
    bool getIgnoreComments();
    void setIgnoreComments(const bool& value);
    void setAsComment(const string& sectName, const string& key);
    void setAsNormal(const string& sectName, const string& key);
    //TODO: Rückgabewert hinzufügen, das Rename* erfolgreich
    bool renameKey(string sectName, const string& oldKey, string newKey);
    bool renameKeyInAllSections(const string& oldKey, string newKey);
    bool renameSection(const string& oldName, const string newName);
    //TODO: bool existsKey(const string& sectName, const string& key);
    //TODO: void deleteAllKeys();
    //TODO: void deleteAllSections();
    
    size_t getSectionCount();
    vector<string> getSectionKeys(const string& sectName);

    string toString();
};
