# Drivak Development Roadmap

## [X] Phase 1: Core Systems & Modding Foundation (Completed)

[X] GameDB: Relational JSON database system with load/save

[X] SaveUtils & DataPaths: Dynamic save/config/mod/script folder resolution

[X] Modding System:

[X] Embedded CPython integration (raw API)

[X] entry.py mod loader

[X] Manual mod writing instead of source copying

[X] Linux .local/share/Drivak runtime installation

## [] Phase 2: Config System + Beginner-Focused UX (Completed/In Progress)

[X] Parse modinfo.json into GameDB (mod tracking metadata)

[] Basic UI with Vulkan Rendering & Qt6

[X] config.py support (user-editable Python-based config)

[X] Stores toggles like copy_default_mods = False

[X] First-launch logic with beginner-safe prompts:

[X] Ask user if they want default mods/scripts

[] Show beginner tips or README popup on first launch

[X] Add theme/appearance options in config (for future UI system)

## [] Phase 3: PDF Rulebook Viewer Integration

[] Add optional Qt PDF viewer:

[] QPdfDocument and QPdfView integrated into a screen/panel

[] Load local rulebooks (Pathfinder, D&D PDFs)

[] Toggle PDF feature in config.py

[] Planned Features for PDF Viewer:

    [] Search (with highlight)

    [] Zoom, scroll, and sidebar outline

    [] Bookmarking or tagging rules

## [] Phase 4: Character Management Tools (Players)

[] Add editable character sheets (stored in DB)

[] Support custom fields per campaign

[] JSON + modifiable via script

[] Quick duplication & version history for characters

[] Export characters as auto-formatted .pdf (JSON already stored in DB)

## [] Phase 5: Modular UI System (Qt + Vulkan-ready)

[] Screens:

    [] TitleScreen

    [] ModScreen

    [] CharacterEditor

    [] RulebookViewer

[] Flexible UI element system (buttons, panels, etc.)

[] Modding hooks for UI + theming (scripts can define panels, override styles)

## [] Phase 6: DM-Focused Tools

[] Link characters, maps, and notes to specific campaigns

[] Campaign timeline / session tracker

[] DM-only notes per session or player

[] Session prep checklist generator

[] Character version history (with DM notes)

[] Secret flags (DM-only info per character)

[] Export player-facing handouts (based on lore or items)

[] NPC impersonation tool (quick notes & dialogue)

[] Faction tracker with influence or relationship levels

[] Encounter builder with loot/NPC links

[] Custom roll tables (loot, quests, story events)

[] Dynamic knowledge visibility ("who knows what") system

## [] Phase 7: Public Launch

[] Itch.io soft launch ($0–$15) — initial testing + user feedback

[] Bug fixes, additional polish, and added features from community feedback

[] Steam launch (v1.0) — $25 final price (required by Steam after Early Access)

This roadmap is a living document. Features and priorities may shift as development progresses and community feedback is integrated.

## ## Phase 8: Android Companion App (Post-Launch)

[] Android build using Qt/QML + shared C++ backend

[] Character management (view/edit/export sheets)

[] Rulebook viewer with local PDFs

[] Session planner: prep checklist, DM notes, NPC quick access

[] Mod/script sync via shared folder or cloud