#include "CJaffaVM.h"

SysVM::CPage SysVM::CPageArray::getCPage(CPageIndex index)
{
	return *(CPage*)(arr[index]);
}

SysVM::Character SysVM::CPageArray::getCharacter(CPageIndex index)
{
	return *(Character*)((*(CPage*)(arr[index])).Value);
}