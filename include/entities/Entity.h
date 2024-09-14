#pragma once

#include <memory>
#include <vector>

#include "components/IComponent.h"
#include "components/Monobehavior.h"

class IComponent;
class Monobehavior;

class Entity {
private:
    std::vector<std::unique_ptr<IComponent>> components;
    std::vector<Monobehavior*> monobehaviors;

public:

    Entity();

    // Function to add a component
    template <typename T, typename... Args>
    T* addComponent(Args&&... args);

    // Method to get a specific type of component
    template <typename T>
    T* getComponent();

    // Update all MonoBehaviour components
    void updateMonoBehaviors();
};
