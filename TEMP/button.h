#pragma once
#include "graphics.h"
#include <functional>

class Button {
public:
    float x, y, width, height;
    std::string label;
    std::function<void()> onClick;

    Button(float x, float y, float width, float height, const std::string& label, std::function<void()> onClick)
        : x(x), y(y), width(width), height(height), label(label), onClick(onClick) {}

    void draw() {
        graphics::Brush br;
        br.fill_color[0] = 0.3f; br.fill_color[1] = 0.6f; br.fill_color[2] = 0.3f;
        br.outline_opacity = 1.0f;
        graphics::drawRect(x, y, width, height, br);

        br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 1.0f; // Text color (white)
        graphics::drawText(x - width / 2, y - 10, label.c_str(), br);
    }

    bool isClicked() {
        graphics::MouseState mouse = graphics::getMouseState();
        return mouse.x >= x - width / 2 && mouse.x <= x + width / 2 &&
            mouse.y >= y - height / 2 && mouse.y <= y + height / 2 &&
            mouse.button_left == 1;  // Left mouse button clicked
    }
};
