#pragma once

#include "powerupbase.h" 

class PowerUpLevel4 : public PowerUpBase {
public:
    PowerUpLevel4(GameState* state, int x, int y);

    void applyEffect() override; 
    void draw() override;        
};
