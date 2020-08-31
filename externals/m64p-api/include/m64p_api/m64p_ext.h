#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "m64p_types.h"

#include <stddef.h>
#include <stdint.h>

typedef void(*ExtCallback)();
typedef void(*ExtIntCallback)(int);

enum ExtCoreEvent {
	ExtCoreEvent_ChangeWindow,
	ExtCoreEvent_StateSetSlot,
	ExtCoreEvent_Stop,
	ExtCoreEvent_StateSave,
	ExtCoreEvent_StateLoad,
	ExtCoreEvent_StateIncSlot,
	ExtCoreEvent_SoftReset,
	ExtCoreEvent_SpeedDown,
	ExtCoreEvent_SpeedUp,
	ExtCoreEvent_TakeNextScreenshot,
	ExtCoreEvent_TogglePause,
	ExtCoreEvent_VolumeMute,
	ExtCoreEvent_VolumeUp,
	ExtCoreEvent_VolumeDown,
	ExtCoreEvent_SetFastForward,
	ExtCoreEvent_AdvanceOne,
	ExtCoreEvent_SetGameShark,
	ExtCoreEvent_UnsetFastForward,
	ExtCoreEvent_UnsetGameShark
};

typedef void(*ptr_ExtSetFrameCallback)(ExtCallback cb);
typedef void(*ptr_ExtSetVICallback)(ExtCallback cb);
typedef void(*ptr_ExtSetResetCallback)(ExtIntCallback cb);
typedef void(*ptr_ExtSetPauseLoopCallback)(ExtCallback cb);
typedef void(*ptr_ExtSetCoreEventCallback)(ExtIntCallback cb);
typedef uint8_t*(*ptr_ExtGetDRAMPtr)(void);
typedef size_t(*ptr_ExtGetDRAMSize)(void);
typedef uint8_t*(*ptr_ExtGetROMPtr)(void);
typedef size_t(*ptr_ExtGetROMSize)(void);
typedef void(*ptr_ExtInvalidateCachedCode)(void);
typedef void(*ptr_ExtInputKeyDown)(int mod, int sym);
typedef void(*ptr_ExtInputKeyUp)(int mod, int sym);
typedef void(*ptr_ExtGfxResizeOutput)(int w, int h);
typedef void(*ptr_ExtResetInputPlugin)(void);
typedef uint32_t(*ptr_ExtContGetInput)(int index);
typedef void(*ptr_ExtContSetInput)(int index, uint32_t input);
typedef uint32_t(*ptr_ExtContGetPluginFilter)(int index);
typedef void(*ptr_ExtContSetPluginFilter)(int index, uint32_t filter);

typedef uint8_t(*ptr_ExtRDRAMRead8)(uint32_t addr);
typedef uint16_t(*ptr_ExtRDRAMRead16)(uint32_t addr);
typedef uint32_t(*ptr_ExtRDRAMRead32)(uint32_t addr);
typedef uint8_t*(*ptr_ExtRDRAMReadBuffer)(uint32_t addr, size_t len);
typedef void(*ptr_ExtRDRAMWrite8)(uint32_t addr, uint8_t val);
typedef void(*ptr_ExtRDRAMWrite16)(uint32_t addr, uint16_t val);
typedef void(*ptr_ExtRDRAMWrite32)(uint32_t addr, uint32_t val);
typedef void(*ptr_ExtRDRAMWriteBuffer)(uint32_t addr, uint8_t* buf, size_t len);
typedef uint8_t(*ptr_ExtROMRead8)(uint32_t addr);
typedef uint16_t(*ptr_ExtROMRead16)(uint32_t addr);
typedef uint32_t(*ptr_ExtROMRead32)(uint32_t addr);
typedef uint8_t* (*ptr_ExtROMReadBuffer)(uint32_t addr, size_t len);
typedef void(*ptr_ExtROMWrite8)(uint32_t addr, uint8_t val);
typedef void(*ptr_ExtROMWrite16)(uint32_t addr, uint16_t val);
typedef void(*ptr_ExtROMWrite32)(uint32_t addr, uint32_t val);
typedef void(*ptr_ExtROMWriteBuffer)(uint32_t addr, uint8_t* buf, size_t len);

#ifdef __cplusplus
}
#endif
