#include "Interface.h"

DWORD Interface::GetCJaffaVM()
{
	return *(DWORD*)((DWORD)Interface::moduleBase + 0x3164E8);
}

DWORD Interface::GetCPage(int index)
{
	//DWORD array = *reinterpret_cast<DWORD*>(Interface::GetCJaffaVM()+0x184);
	switch (index)
	{
	case 5056:
		mystuff::CPage<DWORD> a;
		return a.GetValue();
	}
	return 0;
}

