#pragma once
#include "framework.h"

struct Process {
	HMODULE hmodule = NULL;

	void Init();
};