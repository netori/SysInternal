/*****************************************************************//**
 * \file   Interface.h
 * \brief  This headers contains wrappers for the backend that are used by the GUI.
 * 
 * \author netori
 * \date   March 2023
 *********************************************************************/

#pragma once
#include <cstdint>

namespace Interface
{
	uintptr_t GetCJaffaVM();
	uintptr_t GetCPage(int);
	wchar_t* GetCPageName(int);
};