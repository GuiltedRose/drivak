#pragma once
#include <QString>
#include <string>

namespace SaveUtils {
    QString getSavePath(const std::string& filename, bool useAppData = true);
}