#include "gamedb/GameDB.h"
#include "modding/ModLoader.h"
#include "core/DataPaths.h"
#include <iostream>
#include <QCoreApplication>

int main(int argc, char* argv[]) {
    QCoreApplication App(argc, argv);
    DataPaths::ensureALL();

    GameDB db;
    ModLoader loader(&db);

    loader.copyDefaultModsIfEmpty();
    for (const auto& mod : loader.getLoadedMods()) {
        if (mod.on_game_start) {
            PyObject_CallObject(mod.on_game_start, nullptr);
        }
    }
    

    db.saveToFile("saves/test.json");
    return 0;
}
