#pragma once

#include "components/Monobehavior.h"
#include <iostream>

class TestHelloWorld : public Monobehavior {
	void update() override {
		std::cout << "Hello world" << std::endl;
	}
};