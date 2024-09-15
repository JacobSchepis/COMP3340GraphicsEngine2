#pragma once

#include <vector>

class Monobehavior;

class MonobehaviorManager {
public:
	MonobehaviorManager(const MonobehaviorManager&) = delete;

	static MonobehaviorManager& Instance();

	void awake();
	void start();
	void update();

	void addMonobehavior(Monobehavior* monobehavior);
	void removeMonobehavior(Monobehavior* monobehavior);
private:
	MonobehaviorManager();

	std::vector<Monobehavior*> monobehaviors;
};