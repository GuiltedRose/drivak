#pragma once

#include "gamedb/GameDB.h"
#include "core/IgnoreFilter.h"
#include <string>
#include <QJsonObject>

struct LoadedMod {
    std::string name;
    std::string path;
    std::string description;
    std::string version;
};


class ModLoader {
public:
    ModLoader(GameDB* db);
    bool validateModInfo(const QJsonObject& obj, const std::string& folder);
    static void ensureExampleModsInstalled(const std::string& modDirPath);
    bool loadMod(const std::string& folder);
    void loadAllMods();
    static void copyDefaultModsIfEmpty();

    const std::vector<LoadedMod>& getLoadedMods() const;

private:
    GameDB* dbRef = nullptr;
    std::vector<LoadedMod> loadedMods;
    IgnoreFilter ignore; 
};