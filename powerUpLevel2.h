#pragma once

#include "powerUpBase.h" 

class PowerUpLevel2 : public PowerUpBase {
public:
    PowerUpLevel2(GameState* state, int x, int y);

    void applyEffect() override; 
    void draw() override;        
};
