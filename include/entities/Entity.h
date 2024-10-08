#pragma once

#include <memory>
#include <vector>

#include "components/abstract/IComponent.hpp"

struct IComponent;

class Entity {
private:
    std::vector<std::shared_ptr<IComponent>> components;

public:

    Entity();

    template <typename T, typename... Args>
    void addComponent(Args&&... args) {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        component->parent = this;  // Set the parent pointer
        components.push_back(std::move(component));

        addMonobehavior(components.back().get());
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

private:
    void addMonobehavior(void* newComponent);
};
