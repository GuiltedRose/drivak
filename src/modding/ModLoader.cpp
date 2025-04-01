#undef slots
#include <Python.h>
#include "modding/ModLoader.h"
#include "core/DataPaths.h"
#include "modding/example_mods.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <filesystem>
#include <fstream>
#include <iostream>

ModLoader::ModLoader(GameDB* db) : dbRef(db) {}

bool ModLoader::validateModInfo(const QJsonObject& obj, const std::string& folder) {
    if (!obj.contains("name") || !obj.contains("version"))
        return false;

    std::string name = obj["name"].toString().toStdString();
    std::string version = obj["version"].toString().toStdString();
    std::string description = obj.contains("description") ? obj["description"].toString().toStdString() : "";

    LoadedMod mod;
    mod.name = name;
    mod.path = folder;
    mod.version = version;
    mod.description = description;

    loadedMods.push_back(mod);

    if (dbRef) {
        dbRef->addLoadedMod(ModRecord{
            .name = name,
            .version = version,
            .path = folder,
            .description = description
        });
    }

    return true;
}

void ModLoader::ensureExampleModsInstalled(const std::string& modDirPath) {
    std::filesystem::create_directories(modDirPath);

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

    std::string response;
    std::cout << "No mods found. Would you like to install example mods? [Y/n]: ";
    std::getline(std::cin, response);

    if (!response.empty() && (response[0] == 'n' || response[0] == 'N')) {
        qDebug() << "[ModLoader] User skipped example mod installation.";
        return;
    }

    ensureExampleModsInstalled(DataPaths::defaultModPath().toStdString());
    qDebug() << "[ModLoader] Example mods installed at" << modsPath;
}

bool ModLoader::loadMod(const std::string& folder) {
    std::string entryPath = folder + "/entry.py";

    if (!std::filesystem::exists(entryPath)) {
        printf("Mod entry not found: %s\n", entryPath.c_str());
        return false;
    }

    // --- Load and validate modinfo.json if it exists ---
    QString modInfoPath = QString::fromStdString(folder + "/modinfo.json");
    if (QFile::exists(modInfoPath)) {
        QFile file(modInfoPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray data = file.readAll();
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "[ModLoader] Failed to parse modinfo.json:" << parseError.errorString();
            } else if (doc.isObject()) {
                validateModInfo(doc.object(), folder);
            }
        } else {
            qWarning() << "[ModLoader] Could not open modinfo.json at" << modInfoPath;
        }
    }

    // --- Load and run entry.py ---
    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    PyObject* sysPath = PySys_GetObject("path");
    PyObject* modPath = PyUnicode_FromString(folder.c_str());
    PyList_Append(sysPath, modPath);
    Py_DECREF(modPath);

    PyObject* moduleName = PyUnicode_FromString("entry");
    PyObject* module = PyImport_Import(moduleName);
    Py_DECREF(moduleName);

    if (!module) {
        PyErr_Print();
        printf("Failed to import mod: %s\n", folder.c_str());
        return false;
    }

    PyObject* func = PyObject_GetAttrString(module, "main");
    if (func && PyCallable_Check(func)) {
        PyObject* result = PyObject_CallObject(func, nullptr);
        if (!result) {
            PyErr_Print();
        } else {
            Py_DECREF(result);
        }
    }

    Py_XDECREF(func);
    Py_DECREF(module);
    return true;
}

void ModLoader::loadAllMods() {
    QDir modDir(QString::fromStdString(DataPaths::mods().toStdString()));
    for (const auto& entry : modDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        std::string path = modDir.filePath(entry).toStdString();
        loadMod(path);
    }
}

const std::vector<LoadedMod>& ModLoader::getLoadedMods() const {
    return loadedMods;
}
