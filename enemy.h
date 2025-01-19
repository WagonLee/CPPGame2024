#pragma once

#include "interactiveobject.h"

class Enemy : public InteractiveObject {
protected:
    bool isWeak; 
    float inactiveEndTime = 0.0f;   

public:
    Enemy(GameState* state, int x, int y, const std::string& type);

    // Common functionality
    virtual void handleCollision(Player& player) override;
    virtual void setWeak(bool weak); 
    virtual bool getIsWeak() const; 
    bool isInactive = false;  
    void setInactive(float duration);
    bool getIsInactive() const;
    void activate();

    // Virtual destructor
    virtual ~Enemy() = default;
};
