#pragma once
#include <cstdint>
#include "framework.h"

// Created with ReClass.NET 1.2 by KN4CK3R

class Character_In_Battle
{
public:
	char pad_0000[40]; //0x0000
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
	uint32_t N00000071; //0x0078
	uint32_t N00000072; //0x007C
	uint32_t N00000073; //0x0080
	uint32_t N00000074; //0x0084
	uint32_t N00000075; //0x0088
	uint32_t N00000076; //0x008C
	uint32_t N00000077; //0x0090
	uint32_t SPD; //0x0094
	char pad_0098[1448]; //0x0098
}; //Size: 0x0640
static_assert(sizeof(Character_In_Battle) == 0x640);

class CJaffaVM
{
public:
	char pad_0000[16]; //0x0000
	uint32_t ModuleBase; //0x0010
	char pad_0014[152]; //0x0014
	class CFunctions* CFunctions; //0x00AC
	char pad_00B0[180]; //0x00B0
	class CVMFuncList* CVMFuncList; //0x0164
	char pad_0168[20]; //0x0168
	class CPageManager* N00000277; //0x017C
	class CJaffaVM* N00000278; //0x0180
	//class N00000DCC* CPages; //0x0184
	DWORD CPages; //0x0184
	char pad_0188[156]; //0x0188
	class vm_ip* vm_ip; //0x0224
	class N00000EF3* vm_code_segment; //0x0228
	char pad_022C[8]; //0x022C
	uint32_t exit_condition; //0x0234
	char pad_0238[24]; //0x0238
	class vm_topmost* N000002AC; //0x0250
	char pad_0254[4124]; //0x0254
	class N00000AD2* N0000095C; //0x1270
	class N00000AE6* N0000095D; //0x1274
	class N00000AFA* N0000095E; //0x1278
	class N00000B0E* N0000095F; //0x127C
}; //Size: 0x1280
static_assert(sizeof(CJaffaVM) == 0x1280);

class CVMFuncList
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(CVMFuncList) == 0x44);

class CPageManager
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(CPageManager) == 0x44);

class vm_ip
{
public:
	uint16_t bytecode; //0x0000
	uint16_t next; //0x0002
	char pad_0004[8]; //0x0004
}; //Size: 0x000C
static_assert(sizeof(vm_ip) == 0xC);

class vm_topmost
{
public:
	uint32_t N000005B9; //0x0000
	char pad_0004[1088]; //0x0004
}; //Size: 0x0444
static_assert(sizeof(vm_topmost) == 0x444);

class N00000AD2
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00000AD2) == 0x44);

class N00000AE6
{
public:
	char pad_0000[8]; //0x0000
	class N00000B40* N00000AE9; //0x0008
	char pad_000C[120]; //0x000C
}; //Size: 0x0084
static_assert(sizeof(N00000AE6) == 0x84);

class N00000AFA
{
public:
	char pad_0000[8]; //0x0000
	class N00000C96* N00000AFD; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044
static_assert(sizeof(N00000AFA) == 0x44);

class N00000B0E
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00000B0E) == 0x44);

class N00000B40
{
public:
	char pad_0000[1348]; //0x0000
}; //Size: 0x0544
static_assert(sizeof(N00000B40) == 0x544);

class N00000C96
{
public:
	char pad_0000[1092]; //0x0000
}; //Size: 0x0444
static_assert(sizeof(N00000C96) == 0x444);

class N00000DCC
{
public:
	char pad_0000[20]; //0x0000
}; //Size: 0x0014
static_assert(sizeof(N00000DCC) == 0x14);

class CFunctions
{
public:
	char pad_0000[196]; //0x0000
}; //Size: 0x00C4
static_assert(sizeof(CFunctions) == 0xC4);

class N00000EF3
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00000EF3) == 0x44);

