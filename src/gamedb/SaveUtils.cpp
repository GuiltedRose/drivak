#include "gamedb/SaveUtils.h"
#include "core/DataPaths.h"
#include <QDir>

namespace SaveUtils {
    QString getSavePath(const std::string& filename, bool useAppData) {
        if (useAppData) {
            QString baseDir = DataPaths::saves(); // ⬅️ use our new centralized logic
            QDir dir(baseDir);
            if (!dir.exists()) {
                dir.mkpath(".");
            }
            return dir.filePath(QString::fromStdString(filename));
        } else {
            return QString::fromStdString(filename);
        }
    }
}
