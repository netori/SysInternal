# include "Data.h"

// Read a string from a pointer.
std::string Data::ReadString(uintptr_t address)
{
	return std::string(reinterpret_cast<char*>(address));
}
// Write a string to a pointer.
void Data::WriteString(uintptr_t address, std::string value)
{
	strcpy_s(reinterpret_cast<char*>(address), value.length() + 1, value.c_str());
}
// Read a wide string from a pointer.
std::wstring Data::ReadWString(uintptr_t address)
{
	return std::wstring(reinterpret_cast<wchar_t*>(address));
}
// Write a wide string to a pointer.
void Data::WriteWString(uintptr_t address, std::wstring value)
{
	wcscpy_s(reinterpret_cast<wchar_t*>(address), value.length() + 1, value.c_str());
}