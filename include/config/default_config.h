#pragma once
#include <string>

inline const std::string defaultConfigPy = R"(# config.py
# This file controls Drivak's global settings.
# You can override it with your own values.

copy_default_mods = True       # Automatically install example mods on first launch
enable_mods = True             # Allow Python mods to be loaded and executed
show_tips = True               # Show beginner tips on first run
theme = "default"              # Theme to use (can be a custom Python file)
)";
