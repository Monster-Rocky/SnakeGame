#include "SpeedManager.h"

void SpeedManager::ResetBoost() {
    isBoostActive = false;
}

int SpeedManager::CalculateCurrentSpeed(int score) const {
    int baseSpeed = 0;

    if (mode == SpeedMode::Automatic) {
        if (score < 50) {
            baseSpeed = AUTO_MIN_SPEED; // SLOW
        } else if (score < 100) {
            baseSpeed = AUTO_MEDIUM_SPEED; // MEDIUM
        } else {
            // Gradual progression starting at AUTO_MAX_SPEED (70ms)
            // Deduct 1ms for every 10 points (1 food) above 100
            int extraPoints = score - 100;
            int speedReduction = extraPoints / 10;
            baseSpeed = AUTO_MAX_SPEED - speedReduction;
            
            // Limit automatic base speed to 45ms so it doesn't get unplayable
            if (baseSpeed < 45) {
                baseSpeed = 45;
            }
        }
    } else {
        // Manual mode base speed selection
        switch (manualSpeed) {
            case SpeedLevel::Low:    baseSpeed = LOW_SPEED; break;
            case SpeedLevel::Medium: baseSpeed = MEDIUM_SPEED; break;
            case SpeedLevel::High:   baseSpeed = HIGH_SPEED; break;
        }
    }

    int finalSpeed = baseSpeed;

    // Spacebar Boost overrides base speed
    if (isBoostActive) {
        finalSpeed = BOOST_SPEED;
    }

    // Apply absolute max speed floor limit (ensure timer is never faster than ABSOLUTE_MAX_SPEED)
    if (finalSpeed < ABSOLUTE_MAX_SPEED) {
        finalSpeed = ABSOLUTE_MAX_SPEED;
    }

    return finalSpeed;
}

std::wstring SpeedManager::GetSpeedHUDString(int score) const {
    if (isBoostActive) {
        return L"BOOST";
    }

    if (mode == SpeedMode::Automatic) {
        if (score < 50) {
            return L"SLOW";
        } else if (score < 100) {
            return L"MEDIUM";
        } else {
            return L"FAST";
        }
    } else {
        switch (manualSpeed) {
            case SpeedLevel::Low:    return L"LOW";
            case SpeedLevel::Medium: return L"MEDIUM";
            case SpeedLevel::High:   return L"HIGH";
        }
    }
    return L"NORMAL";
}
