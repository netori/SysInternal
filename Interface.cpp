#include "Interface.h"
// Interface will return guaranteed values from the backend.

uintptr_t Interface::GetCJaffaVM()
{
	return static_cast<uintptr_t>(0x4000);
	//return *reinterpret_cast<DWORD*>(System4VM::instance_ptr);
}

uintptr_t Interface::GetCPage(int index)
{
	//DWORD array = *reinterpret_cast<DWORD*>(Interface::GetCJaffaVM()+0x184);
	switch (index)
	{
	case 5056:  
		//todo: fix this shit
		mystuff::CPage<uintptr_t> a;
		return a.GetValue();
	}
	return 0;
}

