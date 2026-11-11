/*
  LICEcap Simplified Chinese language module.
  The original licecap_ui.cpp remains the implementation of the UI.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include "../WDL/swell/swell.h"
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif
#endif

#include "resource.h"
#include "licecap_ui_lang.h"

typedef struct {
  const char *english;
  const char *chinese;
} licecap_translation;

static const licecap_translation s_translations[] = {
  { "Insert...", "插入…" },
  { "Record...", "录制…" },
  { "Stop", "停止" },
  { "Max FPS:", "最高帧率：" },
  { "Size:", "尺寸：" },
  { "x", "×" },
  { "Status (i.e. 640x480 x fps 3:21 etc)", "状态（例如 640×480，3:21，3 帧/秒）" },
  { "Display in animation", "动画显示" },
  { "title frame:", "标题帧：" },
  { "sec", "秒" },
  { "elapsed time", "显示经过时间" },
  { "mouse button press", "显示鼠标按键" },
  { "Big font", "大字体" },
  { "WEBM options...", "WEBM 选项…" },
  { "Control+Alt+P pauses recording", "Ctrl+Alt+P 暂停录制" },
  { ".GIF repeat count (0=infinite):", ".GIF 循环次数（0 = 无限）：" },
  { "Use .GIF transparency for smaller files", "使用 .GIF 透明度以减小文件" },
  { "Automatically stop after", "在以下时间后自动停止" },
  { "seconds", "秒" },
  { "Insert text frame", "插入文字帧" },
  { "Duration:", "持续时间：" },
  { "Alpha:", "透明度：" },
  { "Insert (0)", "插入（0）" },
  { "Close", "关闭" },
  { "Video bitrate:", "视频码率：" },
  { "Audio bitrate:", "音频码率：" },
  { "Ok", "确定" },
  { "Title", "标题" },
  { "PREROLL: %d - ", "预录：%d - " },
  { "Paused - ", "已暂停 - " },
  { " [stopped]", " [已停止]" },
  { " [recording]", " [录制中]" },
  { " [paused]", " [已暂停]" },
  { " @ %.1ffps", " @ %.1f帧/秒" },
  { "[pause]", "[暂停]" },
  { "[unpause]", "[继续]" },
  { "Choose file for recording", "选择录制文件" },
  { NULL, NULL }
};

static int s_use_chinese = -1;

#ifdef _WIN32
#define licecap_strncasecmp _strnicmp
#else
#define licecap_strncasecmp strncasecmp
#endif

static bool licecap_is_chinese_language(const char *language)
{
  if (!language || !*language) return false;
  return !licecap_strncasecmp(language, "zh", 2) ||
         !licecap_strncasecmp(language, "cmn", 3);
}

static bool licecap_detect_chinese()
{
#ifdef _WIN32
  const LANGID lang = GetUserDefaultUILanguage();
  if (PRIMARYLANGID(lang) == LANG_CHINESE) return true;
#elif defined(__APPLE__)
  CFArrayRef languages = CFLocaleCopyPreferredLanguages();
  if (languages && CFArrayGetCount(languages) > 0)
  {
    CFStringRef language = (CFStringRef)CFArrayGetValueAtIndex(languages, 0);
    char value[64] = { 0 };
    if (CFStringGetCString(language, value, sizeof(value), kCFStringEncodingUTF8) &&
        licecap_is_chinese_language(value))
    {
      CFRelease(languages);
      return true;
    }
  }
  if (languages) CFRelease(languages);
#endif

  const char *language = getenv("LC_ALL");
  if (!language || !*language) language = getenv("LC_MESSAGES");
  if (!language || !*language) language = getenv("LANG");
  return licecap_is_chinese_language(language);
}

bool LICEcapUseChinese()
{
  if (s_use_chinese < 0) s_use_chinese = licecap_detect_chinese() ? 1 : 0;
  return s_use_chinese != 0;
}

const char *LICEcapText(const char *english)
{
  if (!english || !LICEcapUseChinese()) return english;
  for (const licecap_translation *item = s_translations; item->english; ++item)
  {
    if (!strcmp(item->english, english)) return item->chinese;
  }
  return english;
}

#ifdef _WIN32
static void licecap_set_text_w(HWND hwnd, const char *text, bool window_text, int id)
{
  wchar_t value[4096];
  if (!text) text = "";
  if (!MultiByteToWideChar(CP_UTF8, 0, text, -1, value, sizeof(value) / sizeof(value[0])) &&
      !MultiByteToWideChar(CP_ACP, 0, text, -1, value, sizeof(value) / sizeof(value[0]))) return;
  if (window_text) SetWindowTextW(hwnd, value);
  else SetDlgItemTextW(hwnd, id, value);
}
#endif

void LICEcapSetDlgItemText(HWND hwnd, int id, const char *text)
{
#ifdef _WIN32
  licecap_set_text_w(hwnd, text, false, id);
#else
  SetDlgItemText(hwnd, id, text);
#endif
}

void LICEcapSetWindowText(HWND hwnd, const char *text)
{
#ifdef _WIN32
  licecap_set_text_w(hwnd, text, true, 0);
#else
  SetWindowText(hwnd, text);
#endif
}

static BOOL licecap_localize_child(HWND hwnd, LPARAM)
{
  char value[512];
  GetWindowText(hwnd, value, sizeof(value));
  const char *translated = LICEcapText(value);
  if (translated != value && strcmp(value, translated)) LICEcapSetWindowText(hwnd, translated);
  return TRUE;
}

void LICEcapLocalizeDialog(HWND hwnd)
{
  if (!LICEcapUseChinese()) return;
  licecap_localize_child(hwnd, 0);
  EnumChildWindows(hwnd, licecap_localize_child, 0);
}
