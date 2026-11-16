#pragma once
#include <string>

enum class SpeedMode {
    Automatic = 0,
    Manual = 1
};

enum class SpeedLevel {
    Low = 0,
    Medium = 1,
    High = 2
};

class SpeedManager {
public:
    // Speed constants (timer interval in milliseconds - smaller is faster)
    static constexpr int LOW_SPEED = 150;
    static constexpr int MEDIUM_SPEED = 100;
    static constexpr int HIGH_SPEED = 60;

    static constexpr int AUTO_MIN_SPEED = 180;    // SLOW
    static constexpr int AUTO_MEDIUM_SPEED = 110; // MEDIUM
    static constexpr int AUTO_MAX_SPEED = 70;     // FAST

    static constexpr int BOOST_SPEED = 35;        // Temporary Spacebar Boost speed
    static constexpr int ABSOLUTE_MAX_SPEED = 25; // Floor limit to ensure game remains playable

    SpeedMode mode = SpeedMode::Automatic;
    SpeedLevel manualSpeed = SpeedLevel::Medium;
    bool isBoostActive = false;

    SpeedManager() = default;

    // Reset boost state (called on game over, pause, resume)
    void ResetBoost();

    // Calculate final game loop timer interval in milliseconds
    int CalculateCurrentSpeed(int score) const;

    // Return the string representing the current speed state for HUD rendering
    std::wstring GetSpeedHUDString(int score) const;
};
