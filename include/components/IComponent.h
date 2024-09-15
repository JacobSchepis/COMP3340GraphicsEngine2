#pragma once

#include "entities/Entity.h"

class Entity;

class IComponent {
public:
    IComponent() {};

    Entity* parent = nullptr;

    virtual ~IComponent() {};
};