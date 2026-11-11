#ifndef LICECAP_UI_LANG_H
#define LICECAP_UI_LANG_H

bool LICEcapUseChinese();
const char *LICEcapText(const char *english);
void LICEcapLocalizeDialog(HWND hwnd);
void LICEcapSetDlgItemText(HWND hwnd, int id, const char *text);
void LICEcapSetWindowText(HWND hwnd, const char *text);

#endif
