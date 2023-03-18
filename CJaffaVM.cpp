#include "CJaffaVM.h"

CPage CPageArray::getCPage(CPageIndex index)
{
	return *(CPage*)(arr[index]);
}

Character CPageArray::getCharacter(CPageIndex index)
{
	return *(Character*)((*(CPage*)(arr[index])).Value);
}