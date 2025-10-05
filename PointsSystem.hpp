#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

class PointsSystem {
public:
    PointsSystem(const std::string& highScoreFilename = "snake_highscore.txt")
        : score(0),
          highScore(0),
          comboCount(0),
          multiplier(1.0f),
          basePoints(10),
          speedBonusFactor(2.0f),      
          maxMultiplier(3.0f),
          comboWindowMs(1500),        
          highScoreFile(highScoreFilename)
    {
        loadHighScore();
    }

    void onFoodEaten(float speed) {
        using namespace std::chrono;
        auto now = steady_clock::now();
        long long dt_ms = 0;
        if (lastFoodTime.time_since_epoch().count() != 0) {
            dt_ms = duration_cast<milliseconds>(now - lastFoodTime).count();
        }

       
        if (dt_ms > 0 && dt_ms <= comboWindowMs) {
            comboCount++;
        } else {
            comboCount = 1;
        }

    
        multiplier = 1.0f + 0.25f * (comboCount - 1); 
        if (multiplier > maxMultiplier) multiplier = maxMultiplier;

    
        int speedBonus = static_cast<int>(speed * speedBonusFactor);
        int pointsGained = static_cast<int>(basePoints * multiplier) + speedBonus;

        score += pointsGained;

        lastFoodTime = now;

        
        if (score > highScore) highScore = score;
    }


    void onSpecialItemEaten(int specialBasePoints, float specialMultiplier = 1.0f) {
        int pts = static_cast<int>(specialBasePoints * specialMultiplier);
        score += pts;
        if (score > highScore) highScore = score;
    
    }

  
    bool saveHighScore() const {
    std::ofstream out(highScoreFile, std::ios::trunc);
    if (!out) return false;
    out << highScore;
    out.close();
    return true;
}

 
    void resetRun() {
        score = 0;
        comboCount = 0;
        multiplier = 1.0f;
        lastFoodTime = std::chrono::steady_clock::time_point();
    }

    void loadHighScorePublic() { loadHighScore(); }
    long long getScore() const { return score; }
    long long getHighScore() const { return highScore; }
    int getComboCount() const { return comboCount; }
    float getMultiplier() const { return multiplier; }

   
    void setBasePoints(int bp) { basePoints = bp; }
    void setSpeedBonusFactor(float f) { speedBonusFactor = f; }
    void setComboWindowMs(int ms) { comboWindowMs = ms; }
    void setMaxMultiplier(float m) { maxMultiplier = m; }

private:
    long long score;
    long long highScore;
    int comboCount;
    float multiplier;

    int basePoints;
    float speedBonusFactor;
    float maxMultiplier;
    int comboWindowMs; 

    std::chrono::steady_clock::time_point lastFoodTime;
    std::string highScoreFile;

    void loadHighScore() {
        std::ifstream in(highScoreFile);
        if (!in) {
            highScore = 0;
            return;
        }
        long long hs = 0;
        in >> hs;
        if (in) highScore = std::max(0LL, hs);
        else highScore = 0;
    }
};
