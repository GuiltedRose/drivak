#pragma once
#include "core/ThemeConfig.h"

class ThemeLoader {
public:
    static ThemeConfig load(const std::string& themeName = "default");
};
