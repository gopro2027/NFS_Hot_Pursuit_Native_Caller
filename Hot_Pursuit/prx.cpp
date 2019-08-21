#include "stdafx.h"

#define TOC 0xCBF760
struct opd_s { int sub; int toc; };
typedef unsigned long long ui64;
typedef struct
{
	float x;
	float y;
	float z;
} Vector3;
#include <ppu_asm_intrinsics.h>
#include <cellstatus.h>
#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <sys/process.h>

#include <ppu_intrinsics.h>
#include <stdarg.h>
#include <stdio.h>
#include <sysutil\sysutil_msgdialog.h>
#include <sysutil\sysutil_oskdialog.h>
#include <sysutil\sysutil_oskdialog_ext.h>
#include <cell/cell_fs.h>
#include <vec_types.h>
#include <vec_types_snc.h>
//#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include "pad.h"

void sleep(usecond_t time)  //1 second = 1000
{
	sys_timer_usleep(time * 1000);
}


#include "mods.h"
#include "Natives.h"

SYS_MODULE_INFO( Hot_Pursuit, 0, 1, 1);
SYS_MODULE_START( _Hot_Pursuit_prx_entry );

SYS_LIB_DECLARE_WITH_STUB( LIBNAME, SYS_LIB_AUTO_EXPORT, STUBNAME );
SYS_LIB_EXPORT( _Hot_Pursuit_export_function, LIBNAME );

struct gameData {
	bool runTestCode;
	bool runTestCode1;
	bool runTestCode2;
	bool runTestCode3;
};

gameData *GAME_DATA = (gameData*)0x10020000;



void hook() {
	if (GAME_DATA->runTestCode) {
		GAME_DATA->runTestCode = false;
		char string = GetPlayerVehicleIndex();
		printf("Player Vehicle Index : %s\n", string);
	}

	if (GAME_DATA->runTestCode1) {
		GAME_DATA->runTestCode1 = false;

		printf("Setting infinite nitrus!\n");

		SetInfiniteNitrous(GetPlayerVehicleIndex(),true);
	}

	if (GAME_DATA->runTestCode2) {
		GAME_DATA->runTestCode2 = false;
		printf("blank for now.");
		//printf("Dumping natives!\n");
		//dumpNativeTables();
	}

	if (GAME_DATA->runTestCode3) {
		GAME_DATA->runTestCode3 = false;

		printf("Set Vehicle cannot Reck\n");
		SetVulnerableToWrecking(GetPlayerVehicleIndex(), false);
		
	}
}
#define NOP 0



int hookSpecial(unsigned int r3, unsigned int r4) {
	unsigned int ret = call<unsigned int>(0x58B038)(r3, r4);//call the func

	hook();
	
	return ret;
}

void Thread(uint64_t nothing) {
	printf("Modded Thread Started!\n");
	//setup hook
	*(ui64*)(0xC92D48) = *(ui64*)((int)hookSpecial);

	printf("Done setup!\n");
	for (;;) {
		sleep(1000);
		
	
	//	monitorbuttons();
		
		
	}
	
}

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _Hot_Pursuit_export_function(void)
{
    return CELL_OK;
}


extern "C" int _Hot_Pursuit_prx_entry(void)
{
	
	sys_ppu_thread_t rcethread;
	sys_ppu_thread_create(&rcethread, Thread, 0, 0x5AA, 4096/*0x1000*/*7, 0, "Main Thread");
    return SYS_PRX_RESIDENT;
}
