/*****************************************************************//**
 * \file   Interface.h
 * \brief  This headers contains wrappers for the backend that are used by the GUI.
 * 
 * \author netori
 * \date   March 2023
 *********************************************************************/

#pragma once
#include <cstdint>
#include "sys43vm.h"

namespace Interface
{
	uintptr_t GetCJaffaVMAdress();
	uintptr_t GetCPage(int);
	sys43vm::Character* GetCharacter(int);
	wchar_t* GetCPageName(int);
};