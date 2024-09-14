#pragma once

class Entity;

class IComponent {
public:
    IComponent(Entity* parent) : parent(parent) {};

    Entity* parent = nullptr;

    virtual ~IComponent() {};
};