#include "entities/Entity.h"

#include "components/Transform.h"

Entity::Entity() {
	addComponent<Transform>(this);
}

void Entity::updateMonoBehaviors() {
    for (Monobehavior* behavior : monobehaviors) {
        behavior->update();  // Call update directly on each MonoBehaviour
    }
}

template <typename T, typename... Args>
T* Entity::addComponent(Args&&... args) {
    T* component = new T(std::forward<Args>(args)...);
    components.emplace_back(component);

    // Check if the component is a MonoBehaviour
    if (auto behavior = dynamic_cast<Monobehavior*>(component)) {
        monobehaviors.push_back(behavior);
    }

    return component;
}

template <typename T>
T* Entity::getComponent() {
    for (auto& component : components) {
        if (auto target = dynamic_cast<T*>(component.get())) {
            return target;
        }
    }
    return nullptr;
};