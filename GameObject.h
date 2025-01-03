#pragma once

#include <string>

class GameState; // Forward declaration

class GameObject {
protected:
    static int m_next_id;     // Static ID generator
    GameState* m_state;       // Pointer to game state
    std::string m_name;       // Name of the object
    int m_id;                 // Unique ID for each object
    bool m_active;            // Active state
    bool active = true;       // Placeholder for additional state tracking

public:
    // Constructor
    GameObject(GameState* gs, const std::string& name = "");

    // Virtual destructor
    virtual ~GameObject() {}

    // Pure virtual methods for derived classes
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void init() = 0;

    // Active state management
    bool isActive() const { return m_active; }
    void setActive(bool a) { m_active = a; }

    // Getter for ID (Logger Support)
    int getID() const { return m_id; }

    // Getter for name
    std::string getName() const { return m_name; }
};
