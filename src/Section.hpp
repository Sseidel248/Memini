#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <variant>
#include "KeyValue.hpp"

using namespace std;

class Section{
private:
    string m_name;
    vector<KeyValue> m_content;
    bool m_ignoreComments;
    void setSection(string& newSectName);

public:
    Section(string& name);
    Section();
    void addEntry(const string& line);
    void addEntry(string& key, const multiType& value);
    void addEntryWithMultiValues(string& key, vector<multiType>& values);
    void addEntries(vector<string> newEntries);
    void deleteEntry(const string& key);
    void clear();
    vector<KeyValue> getSection();
    multiType* getValue(const string& key);
    void setValue(string& key, const multiType& value);
    KeyValue* getEntry(const string& key);
    string getName();
    void setName(string& name);
    bool existKey(const string& key);
    bool getIgnoreComments();
    void setIgnoreComments(const bool value);
    void setAsComment(const string& key);
    void setAsNormal(const string& key);
    void renameKey(const string& oldKey, string& newKey);
    size_t getCount();
    void sortKeys();
    void getKeys(vector<string>& allKeys);

    string toString();
    bool operator<(const Section& other);
};
