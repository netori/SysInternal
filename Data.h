#pragma once
#include <cstdint>
#include <string>

// Helper to read and write data.
namespace Data
{
	// Read data from a pointer.
	template <typename T>
	T Read(uintptr_t address)
	{
		return *reinterpret_cast<T*>(address);
	}
	// Write data to a pointer.
	template <typename T>
	void Write(uintptr_t address, T value)
	{
		*reinterpret_cast<T*>(address) = value;
	}
	// Read a string from a pointer.
	std::string ReadString(uintptr_t address);	
	// Write a string to a pointer.
	void WriteString(uintptr_t address, std::string value);
	// Read a wide string from a pointer.
	std::wstring ReadWString(uintptr_t address);
	// Write a wide string to a pointer.
	void WriteWString(uintptr_t address, std::wstring value);

}