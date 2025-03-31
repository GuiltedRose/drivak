#pragma once

#include <string>
#include "gamedb/GameDB.h"
#include "core/IgnoreFilter.h"

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