# LICEcap runtime language selection

This change keeps licecap_ui.cpp as the default English UI and adds
licecap_ui_zh.cpp as a runtime language module.

At startup, the module checks the Windows UI language, macOS preferred
languages, and finally LC_ALL/LC_MESSAGES/LANG. A zh or cmn language selects
Simplified Chinese; all other languages keep the original English strings.
