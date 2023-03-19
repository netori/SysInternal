#pragma once

namespace mystuff {
	template <typename T>
	class CPage
	{
	public:
		T GetValue() 
		{ 
			return *reinterpret_cast<DWORD*>(0x083AF3E0);//reinterpret_cast<T*> Value; 
		}
	private:
		char pad_0000[8]; //0x0000
		T* Value; //0x0008
		char pad_000C[100]; //0x000C
	};
}
