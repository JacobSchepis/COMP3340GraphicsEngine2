#include "entities/Entity.h"

#include "components/Transform.h"

Entity::Entity() {
	addComponent<Transform>(this);
}