#pragma once

#include "powerUpBase.h" 

class PowerUpLevel1 : public PowerUpBase {
public:
    PowerUpLevel1(GameState* state, int x, int y);

    void applyEffect() override; 
    void draw() override;        
};
