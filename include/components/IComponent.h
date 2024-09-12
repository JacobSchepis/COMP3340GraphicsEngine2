#pragma once

#include "entities/Entity.h"

class IComponent {
public:
    Entity* parent;

    IComponent(Entity* parent) : parent(parent) {}

    virtual ~IComponent() {}
};