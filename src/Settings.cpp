#include "Settings.h"
#include <windows.h>

Settings::Settings() {
    Load();
}

std::wstring Settings::GetIniPath() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::wstring ws(path);
    size_t pos = ws.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return ws.substr(0, pos + 1) + L"settings.ini";
    }
    return L"settings.ini";
}

void Settings::Load() {
    std::wstring iniPath = GetIniPath();
    
    // Read Sound config (default: ON/1)
    soundOn = GetPrivateProfileIntW(L"Settings", L"Sound", 1, iniPath.c_str()) != 0;
    
    // Read Grid config (default: ON/1)
    gridOn = GetPrivateProfileIntW(L"Settings", L"Grid", 1, iniPath.c_str()) != 0;
    
    // Read Difficulty config (default: Normal/1)
    int diffVal = GetPrivateProfileIntW(L"Settings", L"Difficulty", 1, iniPath.c_str());
    if (diffVal < 0 || diffVal > 2) diffVal = 1;
    difficulty = static_cast<Difficulty>(diffVal);
    
    // Read High Score (default: 0)
    highScore = GetPrivateProfileIntW(L"Data", L"HighScore", 0, iniPath.c_str());

    // Read Speed Mode (default: 1 / Auto)
    speedModeAuto = GetPrivateProfileIntW(L"Settings", L"SpeedModeAuto", 1, iniPath.c_str()) != 0;

    // Read Manual Speed Level (default: 1 / Medium)
    int manSpeedVal = GetPrivateProfileIntW(L"Settings", L"ManualSpeedLevel", 1, iniPath.c_str());
    if (manSpeedVal < 0 || manSpeedVal > 2) manSpeedVal = 1;
    manualSpeedLevel = manSpeedVal;
}

void Settings::Save() {
    std::wstring iniPath = GetIniPath();
    
    WritePrivateProfileStringW(L"Settings", L"Sound", soundOn ? L"1" : L"0", iniPath.c_str());
    WritePrivateProfileStringW(L"Settings", L"Grid", gridOn ? L"1" : L"0", iniPath.c_str());
    
    wchar_t diffStr[16];
    wsprintfW(diffStr, L"%d", static_cast<int>(difficulty));
    WritePrivateProfileStringW(L"Settings", L"Difficulty", diffStr, iniPath.c_str());
    
    wchar_t scoreStr[16];
    wsprintfW(scoreStr, L"%d", highScore);
    WritePrivateProfileStringW(L"Data", L"HighScore", scoreStr, iniPath.c_str());

    WritePrivateProfileStringW(L"Settings", L"SpeedModeAuto", speedModeAuto ? L"1" : L"0", iniPath.c_str());

    wchar_t manSpeedStr[16];
    wsprintfW(manSpeedStr, L"%d", manualSpeedLevel);
    WritePrivateProfileStringW(L"Settings", L"ManualSpeedLevel", manSpeedStr, iniPath.c_str());

    // Flush the INI file cache to disk immediately
    WritePrivateProfileStringW(NULL, NULL, NULL, iniPath.c_str());
}
