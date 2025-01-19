#pragma once

#include "interactiveObject.h"

class GameState;        

class PowerUpBase : public InteractiveObject {
protected:
    GameState* state; 

    int level; 

    bool isCollectible = true;         
    bool visible = true;               
    bool isWeakEffectActive = false;   
    float weakEffectDuration = 0.0f;   
    
    bool markedForRemoval = false; 
    bool inCleanup = false;        

public:
    PowerUpBase(GameState* state, int x, int y, int level);

    bool isMarkedForRemoval() const { return markedForRemoval; }
    void markForRemoval() { markedForRemoval = true; }

    bool isInCleanupProcess() const { return inCleanup; }
    void setInCleanup(bool cleanup) { inCleanup = cleanup; }

    bool canCollide() const { return isCollectible && isActive(); }

    bool isEffectRunning() const {
        return isWeakEffectActive && !isMarkedForRemoval(); 
    }

    virtual void applyEffect() = 0;

    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void init() override;

    void startWeakEffect(float duration);  
    void updateWeakEffect(float dt);       
    void endWeakEffect();                  

    int getLevel() const { return level; }

    virtual void handleCollision(Player& player) override;

    virtual ~PowerUpBase() = default;
};
