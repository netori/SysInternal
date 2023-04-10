/*****************************************************************//**
 * \file   sys43vm.cpp
 * \brief  This file contains the implementations of the sys43 namespace.
 * 
 * \author netori
 * \date   April 2023
 *********************************************************************/

#include "sys43vm.h"
#include "Data.h"

namespace sys43vm
{
	/**
	 * @brief Returns the instance of the CJaffaVM class.
	 * \return 
	 */
	CJaffaVM* CJaffaVM::GetInstance()
	{
		return Data::Read<CJaffaVM*>(hMod + vmFileOffset);
	}

	/**
	* @brief Returns the address of the CJaffaVM instance.
	* \return 
	*/ 	 
	uintptr_t CJaffaVM::GetAddress()
	{
		return Data::Read<uintptr_t>(hMod + vmFileOffset);
	}

	/**
	* @brief Returns the address of a Character instance.
	* \return 
	*/
	Character* CJaffaVM::GetCharacter(int index)
	{
		return std::bit_cast<Character*>(GetInstance()->cpage_arr->arr[index]->Value);
	}
}