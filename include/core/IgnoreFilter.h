#pragma once
#include <QString>
#include <QSet>

class IgnoreFilter {
public:
    IgnoreFilter();
    bool isIgnored(const QString& relativePath) const;

private:
    QSet<QString> ignoredPaths;
    void loadIgnoreFile(const QString& ignorePath);
};
