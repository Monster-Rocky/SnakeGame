#pragma once
#include <string>

enum class Difficulty {
    Easy = 0,
    Normal = 1,
    Hard = 2
};

class Settings {
public:
    bool soundOn = true;
    bool gridOn = true;
    Difficulty difficulty = Difficulty::Normal;
    int highScore = 0;
    bool speedModeAuto = true;
    int manualSpeedLevel = 1; // 0 = Low, 1 = Medium, 2 = High

    Settings();

    // Load configurations and high score from settings.ini
    void Load();

    // Save configurations and high score to settings.ini
    void Save();

private:
    std::wstring GetIniPath();
};
