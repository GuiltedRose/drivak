#include "core/DataPaths.h"
#include "core/example_ignore.h"
#include "config/default_config.h"
#include <QStandardPaths>
#include <QDir>
#include <cstdlib>
#include <filesystem>
#ifdef __linux__
#include <unistd.h>
#endif

static QString g_customBasePath;

namespace DataPaths {
    QString base() {
        if (!g_customBasePath.isEmpty()) return g_customBasePath;
    
        QString root = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        return QDir(root).filePath("Drivak");
    }

    QString join(const QString& sub) {
        return QDir(base()).filePath(sub);
    }

    QString saves() { return join("saves"); }
    QString mods() { return join("mods"); }
    QString scripts() { return join("scripts"); }
    QString logs() { return join("logs"); }
    QString defaultModPath() { return join("mods/default_mod"); }

    inline QString ignoreFiles() { return join(".drivakignore"); }

    void ensureALL() {
        QDir baseDir(base());
        if (!baseDir.exists()) baseDir.mkpath(".");

        QDir().mkpath(saves());
        QDir().mkpath(mods());
        QDir().mkpath(scripts());
        QDir().mkpath(config());
        QDir().mkpath(logs());

        QString ignorePath = ignoreFiles();
        if (!QFile::exists(ignorePath)) {
            QFile file(ignorePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << QString::fromStdString(defaultIgnoreContents);
                file.close();
                qDebug() << "[Drivak] Created default .drivakignore at" << ignorePath;
            }
        }

        QString configPyPath = QDir(config()).filePath("config.py");
        if (!QFile::exists(configPyPath)) {
            QFile file(configPyPath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << QString::fromStdString(defaultConfigPy);
                file.close();
                qDebug() << "[Drivak] Created default config.py at" << configPyPath;
            }
        }
    }

    void overrideBase(const std::string& customPath) {
        g_customBasePath = QString::fromStdString(customPath);
    }

    bool fileExists(const QString& path) {
        return QFile::exists(path);
    }

    QString config() {
    #ifdef __linux__
        QString newConfig = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/drivak";
        QString oldConfig = QDir(base()).filePath("config");

        // If old config exists but new one doesn't — migrate and symlink
        if (QDir(oldConfig).exists() && !QDir(newConfig).exists()) {
            QDir().mkpath(newConfig);
            QDir oldDir(oldConfig);

            for (const QString& file : oldDir.entryList(QDir::Files | QDir::NoDotAndDotDot)) {
                QFile::copy(oldDir.filePath(file), QDir(newConfig).filePath(file));
            }

            // Rename old folder to .config_backup
            QString backup = oldConfig + "_backup";
            QDir().rename(oldConfig, backup);

            // Symlink from old to new
            std::filesystem::create_symlink(newConfig.toStdString(), oldConfig.toStdString());
            qDebug() << "[Drivak] Migrated config to ~/.config/drivak and symlinked from old location.";
        }

        return newConfig;
    #else
        return join("config");
    #endif
    }
}
