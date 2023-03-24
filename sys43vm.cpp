#include "sys43vm.h"
#include "Data.h"

namespace sys43vm
{
	/**
	 * @brief Returns the instance of the CJaffaVM class.
	 * \return 
	 */
	CJaffaVM CJaffaVM::GetInstance()
	{
		return Data::Read<CJaffaVM>(hMod + vmFileOffset);
	}

	/**
	* @brief Returns the address of a CJaffaVM instance.
	* \return 
	*/ 	 
	uintptr_t CJaffaVM::GetAddress()
	{
		return Data::Read<uintptr_t>(hMod + vmFileOffset);
	}
}