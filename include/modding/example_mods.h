#pragma once
#include <string>
#include <vector>

struct ExampleMod {
    std::string filename;
    std::string content;
};

// These are the built-in example mods injected at runtime (if missing).
inline const std::vector<ExampleMod> exampleMods = {
    {
        "hello_mod.py",
        R"(# hello_mod.py
# This script runs when the game starts.
# It's the simplest example of a script-based mod.

def on_game_start():
    print("Hello from hello_mod! You can customize this function.")
)"
    },
    {
        "npc_data.json",
        R"(// npc_data.json
// Example of a data-only mod. This file could be loaded by the game to spawn an NPC.

{
    "npc": {
        "name": "Elira",
        "title": "Wandering Alchemist",
        "location": "Misty Hollow",
        "inventory": ["potion", "herbs", "mystery vial"]
    }
}
)"
    },
    {
        "combo_mod/init.py",
        R"(# combo_mod/init.py
# This mod demonstrates using both scripting and data together.
# It responds when the player enters a specific area.

def on_area_enter(area_name):
    print(f'You have entered: {area_name}')
    if area_name == "Forgotten Cave":
        print("A cold wind chills your spine...")
)"
    },
    {
        "combo_mod/data.json",
        R"(// combo_mod/data.json
// Supplementary data for the combo_mod. This could define area triggers, messages, or other behaviors.

{
    "trigger": "Forgotten Cave",
    "hint": "Bring a torch. Something lurks in the shadows."
}
)"
    }
};
