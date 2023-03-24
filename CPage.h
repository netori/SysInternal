#pragma once

namespace mystuff
{
	template <typename T>
	class CPage
	{
	public:
		char pad_0000[8]; //0x0000
		T* Value; //0x0008
		char pad_000C[100]; //0x000C
		T GetValue()
		{
			return *std::bit_cast<T*>(Value);
		}
	};
};
