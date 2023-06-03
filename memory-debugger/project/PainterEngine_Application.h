#ifndef PAINTERENGINE_APPLICATION_H
#define PAINTERENGINE_APPLICATION_H

#define PX_APPLICATION_MAX_MONITOR_VARIOUS 128
#define PX_APPLICATION_DEFAULE_MONITOR_UPDATEDURATION 300
#define PX_APPLICATION_MEMORYPOOL_SIZE 128*1024

#ifdef __cplusplus
extern "C"{
#endif

#include "PainterEngine_Startup.h"
#ifdef __cplusplus
}
#endif

typedef struct 
{
	px_dword opcode;
	px_dword address;
	px_dword size;
}PX_MemoryViewer_ReadPacket;

typedef struct
{
	px_dword opcode;
	px_dword address;
	px_dword size;
}PX_MemoryViewer_ReadPacketResponseHeader;

typedef struct
{
	px_dword opcode;
	px_dword address;
	px_dword size;
}PX_MemoryViewer_WritePacket;

typedef enum 
{
	PX_APPLICATION_VARIOUS_TYPE_INT8,
	PX_APPLICATION_VARIOUS_TYPE_INT16,
	PX_APPLICATION_VARIOUS_TYPE_INT32,
	PX_APPLICATION_VARIOUS_TYPE_INT64,
	PX_APPLICATION_VARIOUS_TYPE_UINT8,
	PX_APPLICATION_VARIOUS_TYPE_UINT16,
	PX_APPLICATION_VARIOUS_TYPE_UINT32,
	PX_APPLICATION_VARIOUS_TYPE_UINT64,
	PX_APPLICATION_VARIOUS_TYPE_FLOAT32,
	PX_APPLICATION_VARIOUS_TYPE_DOUBLE64,
	PX_APPLICATION_VARIOUS_TYPE_STRING,
	PX_APPLICATION_VARIOUS_TYPE_HEX,
	PX_APPLICATION_VARIOUS_TYPE_BOOL,
}PX_APPLICATION_VARIOUS_TYPE;


typedef enum
{
	PX_APPLICATION_LINKER_TYPE_NONE,
	PX_APPLICATION_LINKER_TYPE_TCP,
	PX_APPLICATION_LINKER_TYPE_UDP,
	PX_APPLICATION_LINKER_TYPE_COM
}PX_APPLICATION_LINKER_TYPE;

typedef struct  
{
	px_char name[32];
	px_dword address;
	px_int size;
	px_bool readonly;
	PX_APPLICATION_VARIOUS_TYPE type;

	union 
	{
		px_bool vboolean;
		px_char vint8;
		px_int16 vint16;
		px_int32 vint32;
		px_int64 vint64;
		px_byte vuint8;
		px_uint16 vuint16;
		px_uint32 vuint32;
		px_uint64 vuint64;
		px_float vfloat;
		px_dword vdouble;
		px_byte  data[128];
		px_char  string[128];
	};

	px_dword elapsed;
	px_dword updateDuration;
}PX_Application_Various;

typedef struct
{
	PX_Object* root;
	PX_Object* menu;
	PX_Object* button_new, * button_delete;
	PX_Object* list;
	PX_Object* various_address;
	PX_Object* various_size;
	PX_Object* button_go;
	PX_Object* memoryview;
	PX_Object *udp_ipbox, * tcp_ipbox,*varbox,*combox;
	PX_Object* messagebox;
	PX_Object* explorer;
	PX_Object_Menu_Item* tcpConnection, * udpConnection;
	px_dword main_address;
	px_int  main_size;
	px_dword main_elapsed;
	PX_Json language;
	PX_Application_Various various[PX_APPLICATION_MAX_MONITOR_VARIOUS];
	PX_TCP tcp;
	PX_UDP udp;
	px_dword ipaddr;
	px_ushort port;
	PX_Runtime runtime;
	PX_APPLICATION_LINKER_TYPE linktype;
	px_byte viewpool[PX_APPLICATION_MEMORYPOOL_SIZE];
}PX_Application;

extern PX_Application App;

px_bool PX_ApplicationInitialize(PX_Application *App,px_int screen_Width,px_int screen_Height);
px_void PX_ApplicationUpdate(PX_Application *App,px_dword elapsed);
px_void PX_ApplicationRender(PX_Application *App,px_dword elapsed);
px_void PX_ApplicationPostEvent(PX_Application *App,PX_Object_Event e);

#endif
