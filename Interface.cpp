/*****************************************************************//**
 * \file   Interface.cpp
 * \brief  This files contains the implementation of the wrappers for the backend that are used by the GUI.
 * 
 * \author netori
 * \date   March 2023
 *********************************************************************/

#include "Interface.h"
#include "sys43vm.h"
#include "CPage.h"
#include "Data.h"

/**
 * @brief Wrapper for sys43vm::CJaffaVM::GetInstance()
 * \return 
 */
uint32_t Interface::GetCJaffaVM()
{
	return 0x1000;
}

uintptr_t Interface::GetCPage(int index)
{
	switch (index)
	{
	case 5056:
		//todo: fix this shit
		mystuff::CPage<uintptr_t> a;
		return a.GetValue();
	}
	return 0;
}