#pragma once

#include <QString>
#include <string>

namespace DataPaths {
    QString base();
    QString saves();
    QString mods();
    QString scripts();
    QString logs();
    QString config();
    QString defaultModPath();
    QString ignoreFiles();


    void ensureALL();
    void overrideBase(const std::string& customPath);
    bool fileExists(const QString& path);
}