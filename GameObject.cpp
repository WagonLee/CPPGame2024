#include "GameObject.h"
#include "GameState.h"

// Initialize static ID generator
int GameObject::m_next_id = 0;

// Constructor
GameObject::GameObject(GameState* gs, const std::string& name)
    : m_state(gs), m_name(name), m_id(++m_next_id), m_active(true) {}
