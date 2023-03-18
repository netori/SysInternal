#pragma once
#include <cstdint>

// Created with ReClass.NET 1.2
namespace SysVM 
{

}
enum CPageIndex : uint32_t
{
	CHR_RANCE			= 5056,
	CHR_KANAMI_KENTOU	= 5088,
	CHR_SHIZUKA_MASOU	= 5117,
	CHR_MARIA_CUSTARD	= 5146,
	CHR_PATTON_MISNARGE = 5175,
	CHR_HUBERT_LIPTON	= 5204,
	CHR_RUSSIAN_CULLET	= 5233,
	CHR_MAITREIA_MEICYAN = 5262,
	CHR_RICK_ADDISON	= 5291,
	CHR_TILDE_SHARP		= 5320,
	CHR_CROOK_MOFUS		= 5349,
	CHR_ALKANESE_RIZE	= 5378,
	CHR_PITTEN_CIAO		= 5407,
	CHR_SENHIME			= 5436,
	CHR_MIRACLE_TOU		= 5465,
	CHR_SANADA_TOURIN	= 5494,
	CHR_PIGU_GELICIAM	= 5523,
	CHR_FREYA_IDUN		= 5552,
	CHR_ROLEX_GADRAS	= 5581,
	CHR_ORUORE_THE_3RD	= 5610,
	CHR_HUNTY_KALAR		= 5639,
};


class Character
{
public:
	uint32_t nID; //0x0000
	char pad_0004[36]; //0x0004
	uint32_t Cur_EVD; //0x0028
	uint32_t Max_EVD; //0x002C
	uint32_t Cur_PRY; //0x0030
	uint32_t Max_PRY; //0x0034
	uint32_t Cur_AMR; //0x0038
	uint32_t Max_AMR; //0x003C
	uint32_t Cur_HP; //0x0040
	uint32_t Max_HP; //0x0044
	uint32_t Cur_SP; //0x0048
	uint32_t Max_SP; //0x004C
	uint32_t Cur_TCT; //0x0050
	uint32_t Max_TCT; //0x0054
	char pad_0058[32]; //0x0058
	uint32_t Min_ATK; //0x0078
	uint32_t Max_ATK; //0x007C
	uint32_t Abs_ATK; //0x0080
	uint32_t Resist; //0x0084
	uint32_t Move; //0x0088
	uint32_t Range; //0x008C
	uint32_t Direct_Hit; //0x0090
	uint32_t SPD; //0x0094
	char pad_0098[248]; //0x0098
}; //Size: 0x0190
static_assert(sizeof(Character) == 0x190);

class CJaffaVM
{
public:
	char pad_0000[16]; //0x0000
	uint32_t mod_base; //0x0010
	char pad_0014[152]; //0x0014
	class CFunctionArray* func_arr; //0x00AC
	char pad_00B0[208]; //0x00B0
	class CJaffaVM* cjaffavm; //0x0180
	class CPageArray* cpage_arr; //0x0184
	char pad_0188[156]; //0x0188
	class VM_IP* vm_ip; //0x0224
	class BytecodeSegment* bytecode_segment; //0x0228
	char pad_022C[8]; //0x022C
	uint32_t exit_condition; //0x0234
	char pad_0238[24]; //0x0238
	class VM_Topmost* vm_topmost; //0x0250
	char pad_0254[1028]; //0x0254
	class VM_Callstack* vm_callstack; //0x0658
	char pad_065C[3236]; //0x065C
}; //Size: 0x1300
static_assert(sizeof(CJaffaVM) == 0x1300);

class VM_IP
{
public:
	uint16_t bytecode; //0x0000
	uint32_t arg;
	//uint16_t arg1; //0x0002
	//uint16_t arg2; //0x0004
}; //Size: 0x0006
static_assert(sizeof(VM_IP) == 0x8);

class VM_Topmost
{
public:
	uint32_t top; //0x0000
}; //Size: 0x0004
static_assert(sizeof(VM_Topmost) == 0x4);

class CPageArray
{
public:
	class CPage* arr[5700]; //0x0000

	CPage getCPage(CPageIndex);
	Character getCharacter(CPageIndex);
	//void* getCPageValue(CPageIndex
	//getCPageValue(CPageIndex);
}; //Size: 0x5910
static_assert(sizeof(CPageArray) == 0x5910);

class CFunctionArray
{
public:
	char pad_0000[72]; //0x0000
}; //Size: 0x0048
static_assert(sizeof(CFunctionArray) == 0x48);

class BytecodeSegment
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(BytecodeSegment) == 0x44);

class VM_Callstack
{
public:
	uint32_t return_to; //0x0000
}; //Size: 0x0004
static_assert(sizeof(VM_Callstack) == 0x4);

class CPage
{
public:
	char pad_0000[8]; //0x0000
	void* Value; //0x0008
	char pad_000C[100]; //0x000C

}; //Size: 0x0070
static_assert(sizeof(CPage) == 0x70);