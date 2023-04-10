/*****************************************************************//**
 * \file   Interface.cpp
 * \brief  This files contains the implementation of the wrappers for the backend that are used by the GUI.
 * 
 * \author netori
 * \date   March 2023
 *********************************************************************/

#include "Interface.h"
#include "sys43vm.h"
#include "Data.h"

/**
 * @brief Wrapper for sys43vm::CJaffaVM::GetAddress()
 * \return 
 */
uint32_t Interface::GetCJaffaVMAdress()
{
	//return 0x1000;
	return sys43vm::CJaffaVM::GetAddress();
}

/**
 * @brief Wrapper for CPage::GetValue()
 * \param index
 * \return 
 */
uintptr_t Interface::GetCPage(int index)
{
	return 0;
}

/**
 * @brief This function retrieves a Character.
 * 
 * \param index used to retrieve a character
 * \return 
 */
sys43vm::Character* Interface::GetCharacter(int index)
{
	// todo: rework the indexing to take enums.
	switch (index)
	{
	case 0: 
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_RANCE);
	case 1:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_KANAMI_KENTOU);
	case 2:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_SHIZUKA_MASOU);
	case 3:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_MARIA_CUSTARD);
	case 4:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_PATTON_MISNARGE);
	case 5:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_HUBERT_LIPTON);
	case 6:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_RUSSIAN_CULLET);
	case 7:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_MAITREIA_MEICYAN);
	case 8:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_RICK_ADDISON);
	case 9:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_TILDE_SHARP);
	case 10:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_CROOK_MOFUS);
	case 11:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_ALKANESE_RIZE);
	case 12:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_PITTEN_CIAO);
	case 13:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_SENHIME);
	case 14:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_MIRACLE_TOU);
	case 15:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_SANADA_TOURIN);
	case 16:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_PIGU_GELICIAM);
	case 17:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_FREYA_IDUN);
	case 18:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_ROLEX_GADRAS);
	case 19:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_ORUORE_THE_3RD);
	case 20:
		return sys43vm::CJaffaVM::GetCharacter(sys43vm::CHR_HUNTY_KALAR);
	}
}
