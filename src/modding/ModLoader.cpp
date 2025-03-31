#define QT_NO_KEYWORDS

#include "modding/ModLoader.h"
#include "core/DataPaths.h"
#include "modding/example_mods.h"
#include <Python.h>
#include <filesystem>
#include <fstream>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>

ModLoader::ModLoader(GameDB* db) : dbRef(db) {}

bool ModLoader::validateModInfo(const QJsonObject& obj, const std::string& folder) {
    QStringList requiredKeys = { "name", "version", "entry" };
    for (const QString& key : requiredKeys) {
        if (!obj.contains(key) || obj.value(key).toString().trimmed().isEmpty()) {
            qWarning() << "[ModLoader] Invalid modinfo.json in" << QString::fromStdString(folder)
                       << "- missing or empty key:" << key;
            return false;
        }
    }
    return true;
}

void ModLoader::ensureExampleModsInstalled(const std::string& modDirPath) {
    for (const auto& mod : exampleMods) {
        std::filesystem::path fullPath = std::filesystem::path(modDirPath) / mod.filename;
        std::filesystem::create_directories(fullPath.parent_path());

        if (!std::filesystem::exists(fullPath)) {
            std::ofstream out(fullPath);
            out << mod.content;
            std::cout << "[Drivak] Installed example mod: " << mod.filename << std::endl;
        }
    }
}

void ModLoader::copyDefaultModsIfEmpty() {
    QString modsPath = DataPaths::mods();
    QDir modDir(modsPath);

    if (!modDir.exists()) QDir().mkpath(modsPath);
    if (!modDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).isEmpty()) return;

    // Check if config.py disables this
    QString configPath = DataPaths::config() + "/config.py";
    if (QFile::exists(configPath)) {
        QFile file(configPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.contains("copy_default_mods = False")) {
                    qDebug() << "[ModLoader] Skipping default mod install (config.py says no)";
                    return;
                }
            }
        }
    }

    // Ask user if they want to install default mods
    std::string response;
    std::cout << "No mods found. Would you like to install example mods? [Y/n]: ";
    std::getline(std::cin, response);

    if (!response.empty() && (response[0] == 'n' || response[0] == 'N')) {
        qDebug() << "[ModLoader] User skipped example mod installation.";
        return;
    }

    // Install example mods into the default mod folder
    std::string defaultModDir = DataPaths::defaultModPath().toStdString();
    ensureExampleModsInstalled(defaultModDir);
    qDebug() << "[ModLoader] Example mods installed at" << QString::fromStdString(defaultModDir);
}

bool ModLoader::loadMod(const std::string& folder) {
    // Normalize and check .drivakignore
    QString relativePath = QString::fromStdString(folder);
    if (relativePath.startsWith(QDir(DataPaths::mods()).absolutePath())) {
        relativePath = "mods/" + QDir(DataPaths::mods()).relativeFilePath(relativePath);
    }

    if (ignore.isIgnored(relativePath)) {
        qDebug() << "[ModLoader] Ignored mod via .drivakignore:" << relativePath;
        return false;
    }

    std::filesystem::path modinfoPath = std::filesystem::path(folder) / "modinfo.json";

    std::string name = "Unknown Mod";
    std::string description = "";
    std::string version = "0.0";
    std::string entryFile = "entry.py";

    // Parse modinfo.json
    if (std::filesystem::exists(modinfoPath)) {
        QFile file(QString::fromStdString(modinfoPath.string()));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                if (!validateModInfo(obj, folder)) return false;

                name = obj.value("name").toString("Unnamed").toStdString();
                description = obj.value("description").toString().toStdString();
                version = obj.value("version").toString("0.0").toStdString();
                entryFile = obj.value("entry").toString("entry.py").toStdString();
            }
        }
    }

    // Check entry file
    std::filesystem::path entryPath = std::filesystem::path(folder) / entryFile;
    if (!std::filesystem::exists(entryPath)) {
        std::cerr << "[ModLoader] Mod entry not found: " << entryPath << std::endl;
        return false;
    }

    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    // Add mod dir to sys.path
    PyObject* sysPath = PySys_GetObject("path");
    PyObject* modPath = PyUnicode_FromString(folder.c_str());
    PyList_Append(sysPath, modPath);
    Py_DECREF(modPath);

    std::string moduleName = entryFile.substr(0, entryFile.find_last_of('.'));
    PyObject* pyModuleName = PyUnicode_FromString(moduleName.c_str());
    PyObject* module = PyImport_Import(pyModuleName);
    Py_DECREF(pyModuleName);

    if (!module) {
        PyErr_Print();
        std::cerr << "[ModLoader] Failed to import module: " << moduleName << std::endl;
        return false;
    }

    LoadedMod mod;
    mod.name = name;
    mod.path = folder;
    mod.description = description;
    mod.version = version;

    mod.on_game_start = PyObject_GetAttrString(module, "on_game_start");
    if (!mod.on_game_start || !PyCallable_Check(mod.on_game_start)) {
        Py_XDECREF(mod.on_game_start);
        mod.on_game_start = nullptr;
    }

    mod.on_area_enter = PyObject_GetAttrString(module, "on_area_enter");
    if (!mod.on_area_enter || !PyCallable_Check(mod.on_area_enter)) {
        Py_XDECREF(mod.on_area_enter);
        mod.on_area_enter = nullptr;
    }

    Py_DECREF(module);
    loadedMods.push_back(mod);

    if (dbRef) {
        dbRef->addLoadedMod(new ModRecord{
            .name = name,
            .version = version,
            .path = folder,
            .description = description
        });
    }

    std::cout << "[ModLoader] Loaded mod: " << name
              << " v" << version
              << (description.empty() ? "" : " - " + description)
              << std::endl;

    return true;
}

void ModLoader::loadAllMods() {
    QDir modDir(QString::fromStdString(DataPaths::mods().toStdString()));
    for (const QString& modFolder : modDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString relPath = "mods/" + modFolder;
        if (ignore.isIgnored(relPath)) {
            qDebug() << "[ModLoader] Skipped (ignored):" << relPath;
            continue;
        }

        std::string fullPath = modDir.filePath(modFolder).toStdString();
        loadMod(fullPath);
    }
}



