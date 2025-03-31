#pragma once

#include <unordered_map>
#include <string>
#include "gamedb/Table.h"

struct ModRecord {
    std::string name;
    std::string version;
    std::string path;
    std::string description;

    bool enabled = true;
};


class GameDB {
public:
    // Access (or create) a table by name
    Table& getTable(const std::string& name);

    // Check if a table exists
    bool hasTable(const std::string& name) const;

    void clear(); // Wipe all tables
    
    bool saveToFile(const std::string& filename, bool useAppData = true) const;
    bool loadFromFile(const std::string& filename, bool useAppData = true); 

    const std::vector<ModRecord>& getLoadedMods() const;
    void addLoadedMod(const ModRecord& mod);

private:
    std::unordered_map<std::string, Table> tables;
    std::vector<ModRecord> loadedMods;
};
