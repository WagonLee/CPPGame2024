#include "GameObject.h"
#include "GameState.h"

int GameObject::m_next_id = 0;

GameObject::GameObject(GameState* gs, const std::string& name)
    : m_state(gs), m_name(name), m_id(++m_next_id), m_active(true) {}
