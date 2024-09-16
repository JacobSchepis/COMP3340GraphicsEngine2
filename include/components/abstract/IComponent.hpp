#pragma once

#include "entities/Entity.h"

class Entity;

struct IComponent {
    IComponent() {};

    Entity* parent = nullptr;

    virtual ~IComponent() {};
};