#pragma once

class Enemy {
public:
    float centerX, centerY, angle;

    Enemy(float cx, float cy);
    void update();
    void draw();
};
