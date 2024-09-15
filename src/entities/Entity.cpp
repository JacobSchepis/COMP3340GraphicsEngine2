#include "entities/Entity.h"

#include "components/Transform.h"
#include "components/Monobehavior.h"

Entity::Entity() 
{
    addComponent<Transform>();
}

void Entity::addMonobehavior(void* newComponent) {
    Monobehavior* monobehavior = dynamic_cast<Monobehavior*>(components.back().get());
    if (monobehavior != nullptr)
    {
        MonobehaviorManager::Instance().addMonobehavior(monobehavior);
    }
}


