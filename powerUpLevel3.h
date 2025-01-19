#pragma once

#include "powerupbase.h" 

class PowerUpLevel3 : public PowerUpBase {
public:
    PowerUpLevel3(GameState* state, int x, int y);

    void applyEffect() override; 
    void draw() override;        
};
