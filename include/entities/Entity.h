#pragma once

#include <memory>
#include <vector>

#include "components/IComponent.h"

class IComponent;

class Entity {
private:
    std::vector<std::unique_ptr<IComponent>> components;

public:

    Entity();

    template <typename T, typename... Args>
    void addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->parent = this;  // Set the parent pointer
        components.push_back(std::move(component));
    }

    // Method to get a specific type of component
    template <typename T>
    T* getComponent() {
        for (auto& component : components) {
            if (auto target = dynamic_cast<T*>(component.get())) {
                return target;
            }
        }
        return nullptr;
    };
};
