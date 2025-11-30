//
// Smart Aimbot System with Humanization
// Designed to be undetectable with natural-looking aim
//

#ifndef AIMBOT_H
#define AIMBOT_H

#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "AntiBan.h"

// Aimbot Configuration
namespace AimbotConfig {
    // Main toggles
    static bool enabled = false;
    static bool aimAssistMode = true;  // Subtle assist vs full aimbot
    
    // Targeting
    static bool targetHead = true;
    static bool targetChest = false;
    static float fov = 120.0f;  // Field of view for targeting
    static float maxDistance = 200.0f;
    
    // Smoothing (higher = smoother/slower, more natural)
    static float smoothing = 8.0f;  // 1-20 range
    static bool adaptiveSmoothing = true;  // Adjust based on distance
    
    // Humanization
    static bool humanize = true;
    static float humanError = 2.0f;  // Pixel error margin
    static bool randomDelays = true;
    static int reactionTime = 150;  // milliseconds
    
    // Advanced
    static float headshotProbability = 0.7f;  // 70% chance to aim for head
    static bool predictMovement = true;
    static bool ignoreKnocked = true;
    static bool visibilityCheck = true;
}

// Target information
struct AimTarget {
    float screenX, screenY;
    float worldX, worldY, worldZ;
    float distance;
    float health;
    bool isHead;
    bool isVisible;
    int priority;
};

// Vector math helpers
struct Vector2 {
    float x, y;
    
    Vector2() : x(0), y(0) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    
    float Length() const {
        return sqrtf(x*x + y*y);
    }
    
    Vector2 Normalize() const {
        float len = Length();
        if (len > 0) return Vector2(x/len, y/len);
        return Vector2(0, 0);
    }
    
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
};

// Calculate angle between two points
float GetAngle(float x1, float y1, float x2, float y2) {
    return atan2f(y2 - y1, x2 - x1) * (180.0f / M_PI);
}

// Calculate distance between two 2D points
float GetDistance2D(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx*dx + dy*dy);
}

// Check if target is within FOV
bool IsInFOV(float centerX, float centerY, float targetX, float targetY, float fov) {
    float distance = GetDistance2D(centerX, centerY, targetX, targetY);
    return distance <= fov;
}

// Add human-like error to aim
Vector2 AddHumanError(Vector2 target) {
    if (!AimbotConfig::humanize) return target;
    
    float errorX = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.0f * AimbotConfig::humanError;
    float errorY = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.0f * AimbotConfig::humanError;
    
    return Vector2(target.x + errorX, target.y + errorY);
}

// Calculate smooth aim adjustment
Vector2 CalculateSmoothAim(Vector2 current, Vector2 target, float smoothing, float deltaTime) {
    Vector2 delta = target - current;
    float distance = delta.Length();
    
    if (distance < 1.0f) return target;
    
    // Adaptive smoothing based on distance
    float adaptiveSmoothness = smoothing;
    if (AimbotConfig::adaptiveSmoothing) {
        // Slower when far, faster when close
        adaptiveSmoothness = smoothing * (1.0f + (distance / 100.0f));
    }
    
    // Calculate smooth step
    float step = distance / adaptiveSmoothness;
    if (step > distance) step = distance;
    
    Vector2 direction = delta.Normalize();
    Vector2 newPos = current + (direction * step);
    
    // Add human error
    if (AimbotConfig::humanize) {
        newPos = AddHumanError(newPos);
    }
    
    return newPos;
}

// Predict target movement
Vector2 PredictMovement(Vector2 currentPos, Vector2 velocity, float distance) {
    if (!AimbotConfig::predictMovement) return currentPos;
    
    // Simple prediction based on velocity and distance
    float timeToTarget = distance / 500.0f; // Assume bullet speed ~500 units/sec
    return currentPos + (velocity * timeToTarget);
}

// Calculate target priority
int CalculatePriority(const AimTarget& target, float screenCenterX, float screenCenterY) {
    int priority = 0;
    
    // Distance to crosshair (closer = higher priority)
    float distToCrosshair = GetDistance2D(screenCenterX, screenCenterY, 
                                         target.screenX, target.screenY);
    priority += (int)(100.0f - distToCrosshair);
    
    // Distance to target (closer = higher priority)
    priority += (int)(100.0f - (target.distance / 2.0f));
    
    // Headshot bonus
    if (target.isHead) priority += 50;
    
    // Visibility bonus
    if (target.isVisible) priority += 30;
    
    // Low health bonus
    if (target.health < 50.0f) priority += 20;
    
    return priority;
}

// Find best target
AimTarget* FindBestTarget(std::vector<AimTarget>& targets, float screenCenterX, float screenCenterY) {
    if (targets.empty()) return nullptr;
    
    AimTarget* bestTarget = nullptr;
    int highestPriority = -1;
    
    for (auto& target : targets) {
        // Check FOV
        if (!IsInFOV(screenCenterX, screenCenterY, target.screenX, target.screenY, 
                     AimbotConfig::fov)) {
            continue;
        }
        
        // Check distance
        if (target.distance > AimbotConfig::maxDistance) continue;
        
        // Check visibility
        if (AimbotConfig::visibilityCheck && !target.isVisible) continue;
        
        // Calculate priority
        int priority = CalculatePriority(target, screenCenterX, screenCenterY);
        
        if (priority > highestPriority) {
            highestPriority = priority;
            bestTarget = &target;
        }
    }
    
    return bestTarget;
}

// Simulate mouse/touch movement
void SimulateAim(float targetX, float targetY) {
    // TODO: Implement actual touch/mouse input simulation
    // This would use Android input injection or game-specific methods
    
    // Placeholder for actual implementation:
    // - Get current touch/mouse position
    // - Calculate delta to target
    // - Inject input events to move aim
    
    // Example (pseudo-code):
    /*
    float currentX, currentY;
    GetCurrentAimPosition(&currentX, &currentY);
    
    float deltaX = targetX - currentX;
    float deltaY = targetY - currentY;
    
    InjectTouchEvent(currentX + deltaX, currentY + deltaY);
    */
}

// Reaction time simulation
class ReactionTimer {
private:
    unsigned long lastTargetChange;
    int currentDelay;
    bool delayActive;
    
public:
    ReactionTimer() : lastTargetChange(0), currentDelay(0), delayActive(false) {}
    
    void OnTargetChange() {
        lastTargetChange = GetCurrentTimeMs();
        
        if (AimbotConfig::randomDelays) {
            // Random reaction time between 100-300ms
            currentDelay = AimbotConfig::reactionTime + 
                          AntiBan::RandomDelay(-50, 100);
        } else {
            currentDelay = AimbotConfig::reactionTime;
        }
        
        delayActive = true;
    }
    
    bool CanAim() {
        if (!delayActive) return true;
        
        unsigned long elapsed = GetCurrentTimeMs() - lastTargetChange;
        if (elapsed >= currentDelay) {
            delayActive = false;
            return true;
        }
        
        return false;
    }
    
private:
    unsigned long GetCurrentTimeMs() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    }
};

// Main aimbot class
class SmartAimbot {
private:
    ReactionTimer reactionTimer;
    AimTarget* currentTarget;
    Vector2 currentAimPos;
    unsigned long lastUpdate;
    
public:
    SmartAimbot() : currentTarget(nullptr), lastUpdate(0) {
        currentAimPos = Vector2(0, 0);
    }
    
    void Update(std::vector<AimTarget>& targets, float screenCenterX, float screenCenterY) {
        if (!AimbotConfig::enabled) {
            currentTarget = nullptr;
            return;
        }
        
        // Find best target
        AimTarget* newTarget = FindBestTarget(targets, screenCenterX, screenCenterY);
        
        // Check if target changed
        if (newTarget != currentTarget) {
            currentTarget = newTarget;
            if (currentTarget) {
                reactionTimer.OnTargetChange();
            }
        }
        
        // No target
        if (!currentTarget) return;
        
        // Wait for reaction time
        if (!reactionTimer.CanAim()) return;
        
        // Decide on headshot or body shot
        bool aimForHead = AimbotConfig::targetHead;
        if (AimbotConfig::humanize) {
            // Randomize based on probability
            float roll = (float)rand() / (float)RAND_MAX;
            aimForHead = roll < AimbotConfig::headshotProbability;
        }
        
        // Get target position
        Vector2 targetPos(currentTarget->screenX, currentTarget->screenY);
        
        // Calculate delta time
        unsigned long currentTime = GetCurrentTimeMs();
        float deltaTime = (currentTime - lastUpdate) / 1000.0f;
        lastUpdate = currentTime;
        
        // Smooth aim calculation
        Vector2 newAimPos;
        if (AimbotConfig::aimAssistMode) {
            // Aim assist: only help slightly
            float assistStrength = 0.3f; // 30% assistance
            Vector2 delta = targetPos - currentAimPos;
            newAimPos = currentAimPos + (delta * assistStrength);
        } else {
            // Full aimbot with smoothing
            newAimPos = CalculateSmoothAim(currentAimPos, targetPos, 
                                          AimbotConfig::smoothing, deltaTime);
        }
        
        currentAimPos = newAimPos;
        
        // Apply aim
        SimulateAim(newAimPos.x, newAimPos.y);
        
        // Anti-ban delay
        if (AimbotConfig::randomDelays) {
            AntiBan::RandomSleep(10, 30);
        }
    }
    
    Vector2 GetCurrentAimPos() const {
        return currentAimPos;
    }
    
    bool HasTarget() const {
        return currentTarget != nullptr;
    }
    
private:
    unsigned long GetCurrentTimeMs() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    }
};

// Global aimbot instance
static SmartAimbot g_Aimbot;

// Public interface
namespace Aimbot {
    void Update(std::vector<AimTarget>& targets, float screenCenterX, float screenCenterY) {
        g_Aimbot.Update(targets, screenCenterX, screenCenterY);
    }
    
    bool IsActive() {
        return AimbotConfig::enabled && g_Aimbot.HasTarget();
    }
    
    Vector2 GetAimPosition() {
        return g_Aimbot.GetCurrentAimPos();
    }
}

#endif // AIMBOT_H
