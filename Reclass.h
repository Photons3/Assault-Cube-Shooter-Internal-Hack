#pragma once

#include "pch.h"

struct Vector3
{ 
	float x, y, z;
};

class ent
{
public:
	Vector3 headPos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 playerPos; //0x0034
	Vector3 viewAngle; //0x0040
	char pad_004C[44]; //0x004C
	int32_t isJumping; //0x0078
	char pad_007C[124]; //0x007C
	int32_t health; //0x00F8
	char pad_00FC[64]; //0x00FC
	int32_t ammoPistol; //0x013C
	char pad_0140[16]; //0x0140
	int32_t ammoWeap; //0x0150
	char pad_0154[208]; //0x0154
	int8_t isShooting; //0x0224
	char N00000286[7]; //0x0225
	char pad_022C[256]; //0x022C
	int32_t teamId; //0x032C
	char pad_0330[72]; //0x0330
	class weapon* currWeapon; //0x0378
	char pad_037C[36]; //0x037C

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x03A0
static_assert(sizeof(ent) == 0x3A0);

class N00000289
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00000289) == 0x44);

class weapon
{
public:
	char pad_0000[4]; //0x0000
	int32_t weaponId; //0x0004
	class ent* owner; //0x0008
	char pad_000C[56]; //0x000C
}; //Size: 0x0044
static_assert(sizeof(weapon) == 0x44);

class N000002B3
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000002B3) == 0x44);