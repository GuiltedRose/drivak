#include "core/IgnoreFilter.h"
#include "core/DataPaths.h"
#include <QFile>
#include <QTextStream>

IgnoreFilter::IgnoreFilter() {
    loadIgnoreFile(DataPaths::ignoreFiles());
}

void IgnoreFilter::loadIgnoreFile(const QString& ignorePath) {
    QFile file(ignorePath);
    if (!file.exists()) return;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty() && !line.startsWith('#')) {
                ignoredPaths.insert(line);
            }
        }
    }
}

bool IgnoreFilter::isIgnored(const QString& relativePath) const {
    for (const QString& rule : ignoredPaths) {
        if (relativePath.startsWith(rule)) return true;
    }
    return false;
}
