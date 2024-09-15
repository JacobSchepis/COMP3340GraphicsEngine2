#include "systems/MonobehaviorManager.h"

#include "components/Monobehavior.h"

MonobehaviorManager& MonobehaviorManager::Instance()
{
	static MonobehaviorManager monobehaviorManager;
	return monobehaviorManager;
}

void MonobehaviorManager::awake() {
	for (Monobehavior* item : monobehaviors)
		item->awake();
}

void MonobehaviorManager::start() {
	for (Monobehavior* item : monobehaviors)
		item->start();
}

void MonobehaviorManager::update() {
	for (Monobehavior* item : monobehaviors)
		item->update();
}

void MonobehaviorManager::addMonobehavior(Monobehavior* monobehavior) {
	monobehaviors.push_back(monobehavior);
}

void MonobehaviorManager::removeMonobehavior(Monobehavior* monobehavior) {

}

MonobehaviorManager::MonobehaviorManager() {

}