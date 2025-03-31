#include "gamedb/GameDB.h"
#include "gamedb/SaveUtils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardPaths>
#include <QTextStream>
#include <QFile>
#include <QDir>

Table& GameDB::getTable(const std::string& name) {
    return tables[name]; // Creates table if it doesn't exist
}

bool GameDB::hasTable(const std::string& name) const {
    return tables.find(name) != tables.end();
}

bool GameDB::saveToFile(const std::string& filename, bool useAppData) const {
    QString fullPath = SaveUtils::getSavePath(filename, useAppData);

    QFileInfo fileInfo(fullPath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QJsonObject root;
    for (const auto& [tableName, table] : tables) {
        QJsonObject tableObj;
        for (const std::string& rowId : table.listIds()) {
            const Row* row = table.get(rowId);
            if (!row) continue;

            QJsonObject rowObj;
            for (const auto& [field, value] : *row) {
                rowObj[QString::fromStdString(field)] = QString::fromStdString(value);
            }
            tableObj[QString::fromStdString(rowId)] = rowObj;
        }
        root[QString::fromStdString(tableName)] = tableObj;
    }

    QJsonDocument doc(root);
    QFile file(fileInfo.filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool GameDB::loadFromFile(const std::string& filename, bool useAppData) {
    QString fullPath = SaveUtils::getSavePath(filename, useAppData);
    QFile file(fullPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) return false;

    if (!doc.isObject()) return false;
    QJsonObject root = doc.object();

    tables.clear();

    for (const QString& tableName : root.keys()) {
        QJsonObject tableObj = root[tableName].toObject();

        for (const QString& rowId : tableObj.keys()) {
            QJsonObject rowObj = tableObj[rowId].toObject();
            Row row;

            for (const QString& key : rowObj.keys()) {
                row[key.toStdString()] = rowObj[key].toString().toStdString();
            }

            tables[tableName.toStdString()].insert(rowId.toStdString(), row);
        }
    }

    return true;
}

const std::vector<ModRecord>& GameDB::getLoadedMods() const {
    return loadedMods;
}

void GameDB::addLoadedMod(const ModRecord& mod) {
    loadedMods.push_back(mod);
}

void GameDB::clear() {
    tables.clear();
}
