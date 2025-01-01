#pragma once

#include <string>

class GameState; // Forward declaration

class GameObject {
protected:
    static int m_next_id;
    GameState* m_state;
    std::string m_name;
    int m_id;
    bool m_active;

public:
    GameObject(GameState* gs, const std::string& name = "");
    virtual ~GameObject() {}

    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void init() = 0;

    bool isActive() const { return m_active; }
    void setActive(bool a) { m_active = a; }

    int getId() const { return m_id; }
    std::string getName() const { return m_name; }
};
