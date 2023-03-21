#pragma once
#include <cstdint>

// A helper that scans for patterns.
namespace Pattern
{
	// Scan for a pattern.
	uintptr_t Find(const char* pattern, const char* mask);
	// Scan for a pattern with a module name.
	uintptr_t Find(const char* pattern, const char* mask, const char* moduleName);
	// Scan for a pattern with a module base.
	uintptr_t Find(const char* pattern, const char* mask, uintptr_t moduleBase);
}