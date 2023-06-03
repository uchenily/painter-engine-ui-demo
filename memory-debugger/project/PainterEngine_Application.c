#include "PainterEngine_Application.h"
PX_Application App;

px_bool PX_ApplicationExport(PX_Application* pApp)
{
	px_int i;
	PX_Json exportJson;
	px_string buildString;

	if (!PX_JsonInitialize(&pApp->runtime.mp_game, &exportJson))
		return PX_FALSE;

	if(!PX_JsonAddObject(&exportJson, "", "various"))return PX_FALSE;
	for (i = 0; i < PX_COUNTOF(pApp->various); i++)
	{
		if (pApp->various[i].name[0])
		{
			px_char content[64];
			px_char temp[16];
			if (!PX_JsonAddObject(&exportJson, "various", pApp->various[i].name))goto _CLEAR;

			//address
			PX_sprintf1(content, sizeof(content), "various.%1", PX_STRINGFORMAT_STRING(pApp->various[i].name));
			PX_itoa(pApp->various[i].address, temp, sizeof(temp), 16);
			if(!PX_JsonAddString(&exportJson, content, "address", temp))goto _CLEAR;;

			//size
			if (!PX_JsonAddNumber(&exportJson, content, "size", pApp->various[i].size))goto _CLEAR;;

			//type
			if (!PX_JsonAddNumber(&exportJson, content, "type", (px_int)pApp->various[i].type))goto _CLEAR;;

			//readonly
			if (!PX_JsonAddBoolean(&exportJson, content, "readonly", (px_int)pApp->various[i].readonly))goto _CLEAR;;

		}
	}

	if (PX_StringInitialize(&pApp->runtime.mp_game, &buildString))
	{
		if (PX_JsonBuild(&exportJson, &buildString))
		{
			//////////////////////////////////////////////////////////////////////////
			PX_SaveDataToFile(buildString.buffer, PX_strlen(buildString.buffer) + 1, "config.json");
		}
	}
	
	PX_StringFree(&buildString);
	PX_JsonFree(&exportJson);
	return PX_TRUE;
_CLEAR:
	PX_JsonFree(&exportJson);
	return PX_FALSE;
}

px_bool PX_ApplicationImport(PX_Application* pApp,const px_char content[])
{

	PX_Json importJson;

	PX_memset(pApp->various, 0, sizeof(pApp->various));
	if (!PX_JsonInitialize(&pApp->runtime.mp_game, &importJson))
		return PX_FALSE;

	if (PX_JsonParse(&importJson,content))
	{
		PX_Json_Value* pValue = PX_JsonGetValue(&importJson, "various");
		if (pValue&&pValue->type==PX_JSON_VALUE_TYPE_OBJECT)
		{
			px_int j;
			for (j=0;j< pValue->_object.values.size;j++)
			{
				px_dword address=0;
				px_int size=0;
				PX_APPLICATION_VARIOUS_TYPE type=PX_APPLICATION_VARIOUS_TYPE_INT8;
				px_bool b=PX_FALSE;
				PX_Json_Value *psubValue,*pAttribValue;
				psubValue = PX_LISTAT(PX_Json_Value, &pValue->_object.values, j);
				pAttribValue = PX_JsonGetObjectValue(psubValue, "address");
				if (pAttribValue&&pAttribValue->type==PX_JSON_VALUE_TYPE_STRING)
				{
					address = PX_htoi(pAttribValue->_string.buffer);
				}

				pAttribValue = PX_JsonGetObjectValue(psubValue, "size");
				if (pAttribValue && pAttribValue->type == PX_JSON_VALUE_TYPE_NUMBER)
				{
					size = (px_int)pAttribValue->_number;
				}

				pAttribValue = PX_JsonGetObjectValue(psubValue, "type");
				if (pAttribValue && pAttribValue->type == PX_JSON_VALUE_TYPE_NUMBER)
				{
					type = (PX_APPLICATION_VARIOUS_TYPE)((px_int)pAttribValue->_number);
				}

				pAttribValue = PX_JsonGetObjectValue(psubValue, "readonly");
				if (pAttribValue && pAttribValue->type == PX_JSON_VALUE_TYPE_BOOLEAN)
				{
					b = pAttribValue->_boolean;
				}
				PX_strcpy(pApp->various[j].name, psubValue->name.buffer, sizeof(pApp->various[j].name));
				pApp->various[j].address = address;
				pApp->various[j].readonly = b;
				pApp->various[j].size = size;
				pApp->various[j].type = type;
			}
			
		}
	}

	PX_JsonFree(&importJson);
	return PX_TRUE;
}

px_void PX_ApplicationOnMenuExport(px_void* ptr)
{
	PX_ApplicationExport((PX_Application*)ptr);
}

px_void PX_ApplicationUpdateList(PX_Application* pApp)
{
	px_int i;
	PX_Object_ListClear(pApp->list);
	for (i=0;i<PX_COUNTOF(pApp->various);i++)
	{
		if (pApp->various[i].name[0])
		{
			PX_Object_ListAdd(pApp->list, pApp->various + i);
		}
	}
}

px_void PX_Application_ItemOnRender(px_surface *psurface,PX_Object *pObject,px_dword elapsed)
{
	PX_Application_Various *pDesc=PX_Object_ListItemGetData(pObject);
	PX_Application* pApp = (PX_Application*)pObject->User_ptr;
	px_int x, y, w, h;
	px_float inheritX, inheritY;
	px_int dx;
	px_char content[128] = {0};
	PX_ObjectGetInheritXY(pObject, &inheritX, &inheritY);
	x = (px_int)(pObject->x + inheritX);
	y = (px_int)(pObject->y + inheritY);
	w = (px_int)pObject->Width;
	h = (px_int)pObject->Height;
	dx = x + 6;

	dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, pDesc->name, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
	if (pDesc->readonly)
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "( readonly, 0x", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
	else
	dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "( 0x", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
	PX_itoa(pDesc->address, content, sizeof(content), 16);
	dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
	dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, ", ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
	PX_itoa(pDesc->size, content,sizeof(content),10);
	dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
	dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, " bytes, ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
	switch (pDesc->type)
	{
	case PX_APPLICATION_VARIOUS_TYPE_INT8:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "int8): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT(pDesc->vint8));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_INT16:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "int16): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT(pDesc->vint16));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_INT32:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "int32): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT(pDesc->vint32));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_INT64:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "int64): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT((px_int)pDesc->vint64));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_UINT8:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "uint8): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT(pDesc->vuint8));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_UINT16:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "uint16): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT(pDesc->vuint16));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_UINT32:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "uint32): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT(pDesc->vuint32));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_UINT64:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "uint64): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_INT((px_int)pDesc->vuint64));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_FLOAT32:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "float32): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1.6", PX_STRINGFORMAT_FLOAT(pDesc->vfloat));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_DOUBLE64:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "double64): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1.8", PX_STRINGFORMAT_FLOAT((px_float)pDesc->vdouble));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_STRING:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "string): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		PX_sprintf1(content, sizeof(content), "%1", PX_STRINGFORMAT_STRING(pDesc->string));
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_HEX:
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "hex): ", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		if (pDesc->size>PX_COUNTOF(content)/2-1)
			PX_BufferToHexString(pDesc->data, PX_COUNTOF(content) / 2 - 1, content);
		else
			PX_BufferToHexString(pDesc->data, pDesc->size, content);
		dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, content, PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		break;
	case PX_APPLICATION_VARIOUS_TYPE_BOOL:
		if (pDesc->vboolean)
		{
			dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "boolean):TRUE", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		}
		else
		{
			dx += PX_FontModuleDrawText(psurface, &pApp->runtime.fontmodule, dx, y + h / 2, PX_ALIGN_LEFTMID, "boolean):FALSE", PX_OBJECT_UI_DEFAULT_FONTCOLOR);
		}
	default:
		break;
	}

}

px_bool PX_Application_ItemOnCreate(px_memorypool* mp, PX_Object* ItemObject, px_void* userptr)
{
	ItemObject->Func_ObjectRender = PX_Application_ItemOnRender;
	ItemObject->User_ptr = userptr;
	return PX_TRUE;
}

px_void PX_ApplicationOnMenuUDPNetworkConnectionCallback(px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_IPBoxShow(pApp->udp_ipbox);
}

px_void PX_ApplicationOnMenuTCPNetworkConnectionCallback(px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_IPBoxShow(pApp->tcp_ipbox);
}

px_void PX_ApplicationOnMenuCOMCallback(px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_COMBoxShow(pApp->combox);
}

px_void PX_ApplicationOnMenuAboutCallback(px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_MessageBoxAlertOk(pApp->messagebox, PX_JsonGetString(&pApp->language, "main.about"), PX_NULL, PX_NULL);
}

px_void PX_ApplicationOnNewVariousButtonCallback(PX_Object *pObject,PX_Object_Event e,px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_VarBoxShow(pApp->varbox);
}

px_void PX_ApplicationOnDeleteVariousButtonCallback(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_ListRemoveItem(pApp->list, PX_Object_ListGetCurrentSelectIndex(pApp->list));
}

px_void PX_ApplicationOnGoButtonCallback(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	pApp->main_address = PX_htoi(PX_Object_VariousGetText(pApp->various_address));
	pApp->main_size = PX_atoi(PX_Object_VariousGetText(pApp->various_size));
}

px_void PX_ApplicationOnMemoryChanged(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	typedef struct 
	{
		PX_MemoryViewer_WritePacket writepacket;
		px_byte data;
	}__write_pack;
	
	PX_Application* pApp = (PX_Application*)ptr;
	__write_pack wpack;
	wpack.writepacket.opcode = 1;
	wpack.writepacket.address = pApp->main_address + e.Param_uint[0];
	wpack.writepacket.size = 1;
	wpack.data = e.Param_uint[1];

	switch (pApp->linktype)
	{
	case PX_APPLICATION_LINKER_TYPE_UDP:
	{
		PX_UDP_ADDR addr;
		addr.ipv4 = pApp->ipaddr;
		addr.port = pApp->port;
		PX_UDPSend(&pApp->udp, addr, &wpack, sizeof(wpack));
	}
	break;
	case PX_APPLICATION_LINKER_TYPE_TCP:
	{
		PX_TCP_ADDR addr;
		addr.ipv4 = pApp->ipaddr;
		addr.port = pApp->port;
		if (!PX_TCPSend(&pApp->tcp, &wpack, sizeof(wpack)))
		{
			if (PX_TCPConnect(&pApp->tcp, addr))
			{
				PX_TCPSend(&pApp->tcp, &wpack, sizeof(wpack));
			}
			else
			{
				pApp->linktype = PX_APPLICATION_LINKER_TYPE_NONE;
			}
		}
	}
	break;
	default:
		break;
	}
}

px_void PX_ApplicationOnUDPConfirm(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_IPBox* pBox = PX_ObjectGetDesc(PX_Object_IPBox,pObject);
	pApp->ipaddr = PX_inet_addr(PX_Object_VariousGetText(pBox->various_ip));
	pApp->port = (px_ushort)PX_atoi(PX_Object_VariousGetText(pBox->various_port));
	pApp->port = PX_htons(pApp->port);

	if (PX_UDPInitialize(&pApp->udp,PX_UDP_IP_TYPE_IPV4))
	{
		pApp->udpConnection->enable = PX_FALSE;
		pApp->tcpConnection->enable = PX_FALSE;
		pApp->linktype = PX_APPLICATION_LINKER_TYPE_UDP;
	}

}

px_void PX_ApplicationOnTCPConfirm(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_IPBox* pBox = PX_ObjectGetDesc(PX_Object_IPBox, pObject);
	pApp->ipaddr = PX_inet_addr(PX_Object_VariousGetText(pBox->various_ip));
	pApp->port = (px_ushort)PX_atoi(PX_Object_VariousGetText(pBox->various_port));
	pApp->port = PX_htons(pApp->port);

	if (PX_TCPInitialize(&pApp->tcp, PX_UDP_IP_TYPE_IPV4))
	{
		pApp->udpConnection->enable = PX_FALSE;
		pApp->tcpConnection->enable = PX_FALSE;
		pApp->linktype = PX_APPLICATION_LINKER_TYPE_TCP;
	}

}

px_void PX_ApplicationOnNewVariousCallback(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	px_int i;
	const px_char* Name;
	px_dword address;
	px_int size;
	px_bool readonly;
	PX_APPLICATION_VARIOUS_TYPE type;
	PX_Application* pApp = (PX_Application*)ptr;
	PX_Object_VarBox* pdesc = PX_ObjectGetDesc(PX_Object_VarBox, pObject);
	Name = PX_Object_VariousGetText(pdesc->various_name);
	address = PX_htoi(PX_Object_VariousGetText(pdesc->various_address));
	size = PX_atoi(PX_Object_VariousGetText(pdesc->various_size));
	readonly = PX_Object_CheckBoxGetCheck(pdesc->checkbox_readonly);
	type = (PX_APPLICATION_VARIOUS_TYPE)PX_Object_SelectBarGetCurrentIndex(pdesc->selectbar_type);

	if (PX_strlen(Name)> sizeof(pApp->various[0].name)-1)
	{
		return;
	}

	for (i=0;i<PX_COUNTOF(pApp->various);i++)
	{
		if (PX_strequ(Name,pApp->various[i].name))
		{
			PX_memset(&pApp->various[i].data, 0, sizeof(pApp->various[i].data));
			pApp->various[i].address = address;
			pApp->various[i].size = size;
			pApp->various[i].readonly = readonly;
			pApp->various[i].type = type;
			PX_ApplicationUpdateList(pApp);
			return;
		}
	}

	for (i = 0; i < PX_COUNTOF(pApp->various); i++)
	{
		if (pApp->various[i].name[0]=='\0')
		{
			PX_memset(&pApp->various[i], 0, sizeof(pApp->various[i]));
			PX_strcpy(pApp->various[i].name, Name, sizeof(pApp->various[i].name));
			pApp->various[i].address = address;
			pApp->various[i].size = size;
			pApp->various[i].readonly = readonly;
			pApp->various[i].type = type;
			PX_ApplicationUpdateList(pApp);
			return;
		}
	}
}
px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_Object_Menu_Item* pItem,*pCItem;
	
	PX_zeromemory(pApp, sizeof(PX_Application));
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	//load fontmodule
	if (!PX_FontModuleInitialize(&pApp->runtime.mp_resources, &pApp->runtime.fontmodule))return PX_FALSE;
	if (!PX_LoadFontModuleFromFile(&pApp->runtime.fontmodule, "assets/font.pxf"))return PX_FALSE;
	
	if (!PX_JsonInitialize(&pApp->runtime.mp_resources, &pApp->language))return PX_FALSE;
	if (!PX_LoadJsonFromFile(&pApp->language, "assets/language.json"))return PX_FALSE;

	
	pApp->root = PX_ObjectCreate(&pApp->runtime.mp_ui, 0, 0, 0, 0, 0, 0, 0);
	//tcp ipbox
	pApp->tcp_ipbox = PX_Object_IPBoxCreate(&pApp->runtime.mp_ui, pApp->root, pApp->runtime.surface_width / 2 - 164, pApp->runtime.surface_height / 2 - 96, 328, 192, "connection", &pApp->runtime.fontmodule);
	PX_ObjectRegisterEvent(pApp->tcp_ipbox, PX_OBJECT_EVENT_EXECUTE, PX_ApplicationOnTCPConfirm, pApp);

	//udp ipbox
	pApp->udp_ipbox = PX_Object_IPBoxCreate(&pApp->runtime.mp_ui, pApp->root, pApp->runtime.surface_width / 2 - 164, pApp->runtime.surface_height / 2 - 96, 328, 192, "connection", &pApp->runtime.fontmodule);
	PX_ObjectRegisterEvent(pApp->udp_ipbox, PX_OBJECT_EVENT_EXECUTE, PX_ApplicationOnUDPConfirm, pApp);


	//combox
	pApp->combox = PX_Object_COMBoxCreate(&pApp->runtime.mp_ui, pApp->root, pApp->runtime.surface_width / 2 - 164, pApp->runtime.surface_height / 2 - 192, 328, 384, "com", &pApp->runtime.fontmodule);

	//varbox
	pApp->varbox = PX_Object_VarBoxCreate(&pApp->runtime.mp_ui, pApp->root, pApp->runtime.surface_width / 2 - 164, pApp->runtime.surface_height / 2 - 192, 328, 384, "various", &pApp->runtime.fontmodule);
	PX_ObjectRegisterEvent(pApp->varbox, PX_OBJECT_EVENT_EXECUTE, PX_ApplicationOnNewVariousCallback, pApp);

	//menu
	pApp->menu = PX_Object_MenuCreate(&pApp->runtime.mp_ui, pApp->root, 0, 0, 96, &pApp->runtime.fontmodule);
	pItem = PX_Object_MenuAddItem(pApp->menu, PX_NULL, PX_JsonGetString(&pApp->language, "menu.new connection"), PX_NULL, PX_NULL);
	pApp->tcpConnection=PX_Object_MenuAddItem(pApp->menu, pItem, PX_JsonGetString(&pApp->language, "menu.new tcp/ip connection"), PX_ApplicationOnMenuTCPNetworkConnectionCallback, pApp);
	pApp->udpConnection=PX_Object_MenuAddItem(pApp->menu, pItem, PX_JsonGetString(&pApp->language, "menu.new udp connection"), PX_ApplicationOnMenuUDPNetworkConnectionCallback, pApp);
			pCItem = PX_Object_MenuAddItem(pApp->menu, pItem, PX_JsonGetString(&pApp->language, "menu.new com connection"), PX_ApplicationOnMenuCOMCallback, pApp);
			pCItem->enable = PX_FALSE;

	pItem = PX_Object_MenuAddItem(pApp->menu, PX_NULL, PX_JsonGetString(&pApp->language, "menu.file"), PX_NULL, PX_NULL);
			PX_Object_MenuAddItem(pApp->menu, pItem, PX_JsonGetString(&pApp->language, "menu.open"), PX_NULL, PX_NULL);
			PX_Object_MenuAddItem(pApp->menu, pItem, PX_JsonGetString(&pApp->language, "menu.save"), PX_ApplicationOnMenuExport, pApp);
	pItem = PX_Object_MenuAddItem(pApp->menu, PX_NULL, PX_JsonGetString(&pApp->language, "menu.about"), PX_NULL, PX_NULL);
			PX_Object_MenuAddItem(pApp->menu, pItem, PX_JsonGetString(&pApp->language, "menu.about memory explorer"), PX_ApplicationOnMenuAboutCallback, pApp);

	//new monitor
	pApp->button_new = PX_Object_PushButtonCreate(&pApp->runtime.mp_ui, pApp->root, 0, 0, 20, 20, "+", PX_NULL);
	PX_ObjectRegisterEvent(pApp->button_new, PX_OBJECT_EVENT_EXECUTE, PX_ApplicationOnNewVariousButtonCallback, pApp);
	//delete
	pApp->button_delete = PX_Object_PushButtonCreate(&pApp->runtime.mp_ui, pApp->root, 0, 0, 20, 20, "-", PX_NULL);
	PX_ObjectRegisterEvent(pApp->button_delete, PX_OBJECT_EVENT_EXECUTE, PX_ApplicationOnDeleteVariousButtonCallback, pApp);

	//list
	pApp->list = PX_Object_ListCreate(&pApp->runtime.mp_ui, pApp->root, 8, 52, pApp->runtime.surface_width - 16, pApp->runtime.surface_height/2 - 60, 24, PX_Application_ItemOnCreate, pApp);
	

	//address bar
	pApp->various_address = PX_Object_VariousCreate(&pApp->runtime.mp_ui, pApp->root, 8, pApp->runtime.surface_height / 2, 256, 24, "Memory Address:",PX_OBJECT_VARIOUS_TYPE_EDIT, &pApp->runtime.fontmodule);
	PX_Object_VariousSetEditStyle(pApp->various_address, PX_OBJECT_VARIOUS_EDIT_TYPE_HEX);
	PX_Object_VariousEditSetMaxLength(pApp->various_address, 8);
	
	pApp->various_size = PX_Object_VariousCreate(&pApp->runtime.mp_ui, pApp->root, 8+256+16, pApp->runtime.surface_height / 2, 128, 24, "size:", PX_OBJECT_VARIOUS_TYPE_EDIT, &pApp->runtime.fontmodule);
	PX_Object_VariousSetEditStyle(pApp->various_size, PX_OBJECT_VARIOUS_EDIT_TYPE_INT);
	PX_Object_VariousEditSetMaxLength(pApp->various_size, 8);
	
	pApp->button_go = PX_Object_PushButtonCreate(&pApp->runtime.mp_ui, pApp->root, 8 + 256 + 16+128+16, pApp->runtime.surface_height / 2, 64, 24, PX_JsonGetString(&pApp->language, "main.go"), &pApp->runtime.fontmodule);
	PX_ObjectRegisterEvent(pApp->button_go, PX_OBJECT_EVENT_EXECUTE, PX_ApplicationOnGoButtonCallback, pApp);
	
	//memory viewer
	pApp->memoryview = PX_Object_MemoryViewCreate(&pApp->runtime.mp_ui, pApp->root, 8, pApp->runtime.surface_height / 2 + 32, pApp->runtime.surface_width - 16, pApp->runtime.surface_height / 2 - 32 - 8);
	PX_Object_MemoryViewSetData(pApp->memoryview, pApp->viewpool, sizeof(pApp->viewpool));
	PX_ObjectRegisterEvent(pApp->memoryview, PX_OBJECT_EVENT_VALUECHANGED, PX_ApplicationOnMemoryChanged, pApp);
	//messagebox
	pApp->messagebox = PX_Object_MessageBoxCreate(&pApp->runtime.mp_ui, pApp->root, &pApp->runtime.fontmodule);

	pApp->linktype = PX_APPLICATION_LINKER_TYPE_NONE;

	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp,px_dword elapsed)
{
	px_int i;
	px_int readsize=0;
	PX_Object_List* pList;
	px_byte cacheBuffer[PX_APPLICATION_MEMORYPOOL_SIZE+64];
	PX_MemoryViewer_ReadPacketResponseHeader* presp = (PX_MemoryViewer_ReadPacketResponseHeader*)cacheBuffer;
	PX_ObjectUpdate(pApp->root, elapsed);
	pList = PX_ObjectGetDesc(PX_Object_List, pApp->list);

	//write
	//main
	pApp->main_elapsed += elapsed;
	if (pApp->main_elapsed > PX_APPLICATION_DEFAULE_MONITOR_UPDATEDURATION)
	{
		PX_MemoryViewer_ReadPacket readpacket;
		pApp->main_elapsed = 0;
		readpacket.opcode = 0;
		readpacket.address = pApp->main_address;
		readpacket.size = pApp->main_size;
		switch (pApp->linktype)
		{
		case PX_APPLICATION_LINKER_TYPE_UDP:
		{
			PX_UDP_ADDR addr;
			addr.ipv4 = pApp->ipaddr;
			addr.port = pApp->port;
			PX_UDPSend(&pApp->udp, addr, &readpacket, sizeof(readpacket));
		}
		break;
		case PX_APPLICATION_LINKER_TYPE_TCP:
		{
			PX_TCP_ADDR addr;
			addr.ipv4 = pApp->ipaddr;
			addr.port = pApp->port;
			if (!PX_TCPSend(&pApp->tcp, &readpacket, sizeof(readpacket)))
			{
				if (PX_TCPConnect(&pApp->tcp, addr))
				{
					PX_TCPSend(&pApp->tcp, &readpacket, sizeof(readpacket));
				}
				else
				{
					pApp->linktype = PX_APPLICATION_LINKER_TYPE_NONE;
				}
			}
		}
		break;
		default:
			break;
		}
	}

	//items
	for (i=0;i<pList->pData.size;i++)
	{
		if (PX_Object_ListItemIsOnDisplayPresent(pApp->list,i))
		{
			PX_Application_Various* pVar = PX_Object_ListGetItemData(pApp->list, i);
			pVar->elapsed += elapsed;
			if (pVar->elapsed> PX_APPLICATION_DEFAULE_MONITOR_UPDATEDURATION)
			{
				PX_MemoryViewer_ReadPacket readpacket;
				pVar->elapsed = 0;
				readpacket.opcode = 0;
				readpacket.address = pVar->address;
				readpacket.size = pVar->size;
				switch (pApp->linktype)
				{
				case PX_APPLICATION_LINKER_TYPE_UDP:
				{
					PX_UDP_ADDR addr;
					addr.ipv4 = pApp->ipaddr;
					addr.port = pApp->port;
					PX_UDPSend(&pApp->udp, addr, &readpacket, sizeof(readpacket));
				}
				break;
				case PX_APPLICATION_LINKER_TYPE_TCP:
				{
					PX_TCP_ADDR addr;
					addr.ipv4 = pApp->ipaddr;
					addr.port = pApp->port;
					if (!PX_TCPSend(&pApp->tcp, &readpacket, sizeof(readpacket)))
					{
						if (PX_TCPConnect(&pApp->tcp, addr))
						{
							PX_TCPSend(&pApp->tcp, &readpacket, sizeof(readpacket));
						}
						else
						{
							pApp->linktype = PX_APPLICATION_LINKER_TYPE_NONE;
						}
					}
				}
				break;
				default:
					break;
				}
			}

		}
	}
	//read 
	while (PX_TRUE)
	{
		switch (pApp->linktype)
		{
		case PX_APPLICATION_LINKER_TYPE_UDP:
		{
			PX_UDP_ADDR addr;
			if (!PX_UDPReceived(&pApp->udp, &addr, cacheBuffer, sizeof(cacheBuffer), &readsize))
			{
				readsize = 0;
			}
		}
		break;
		case PX_APPLICATION_LINKER_TYPE_TCP:
		{
			if (!PX_TCPReceived(&pApp->tcp, cacheBuffer, sizeof(cacheBuffer), 1))
			{
				readsize = 0;
			}
		}
		break;
		default:
			break;
		}

		if (!readsize)
		{
			break;
		}

		if (readsize<=sizeof(PX_MemoryViewer_ReadPacketResponseHeader))
		{
			continue;
		}

		if (readsize != presp->size+sizeof(PX_MemoryViewer_ReadPacketResponseHeader))
		{
			continue;
		}

		for (i=0;i<PX_COUNTOF(pApp->various);i++)
		{
			if (pApp->various[i].name[0]&&pApp->various[i].address==presp->address&&pApp->various[i].size==presp->size)
			{
				PX_memcpy(pApp->various[i].data, cacheBuffer + sizeof(PX_MemoryViewer_ReadPacketResponseHeader), presp->size);
			}
		}
		if (pApp->main_address == presp->address && pApp->main_size == presp->size)
		{
			PX_memcpy(pApp->viewpool, cacheBuffer + sizeof(PX_MemoryViewer_ReadPacketResponseHeader), presp->size);
		}

	}

}

px_void PX_ApplicationRender(PX_Application *pApp,px_dword elapsed)
{
	px_surface *pRenderSurface=&pApp->runtime.RenderSurface;
	///////////////////////////////////////////////////////////////////
	//update ui
	pApp->button_new->x = pApp->runtime.surface_width - 8 - 32 * 2.f;
	pApp->button_new->y = 24.f;
	
	pApp->button_delete->x = pApp->runtime.surface_width - 8 - 32 * 1.f;
	pApp->button_delete->y = 24.f;

	pApp->list->x = 8;
	pApp->list->y = 52;
	pApp->list->Width = pApp->runtime.surface_width - 16.f;
	pApp->list->Height = pApp->runtime.surface_height / 2 - 60.f;

	pApp->various_address->y = pApp->runtime.surface_height / 2.f;
	pApp->various_size->y = pApp->runtime.surface_height / 2.f;
	pApp->button_go->y = pApp->runtime.surface_height / 2.f;

	pApp->memoryview->x = 8;
	pApp->memoryview->y = pApp->runtime.surface_height / 2.f + 32;
	pApp->memoryview->Width = pApp->runtime.surface_width - 16.f;
	pApp->memoryview->Height = pApp->runtime.surface_height / 2.f - 32 - 8;


	PX_RuntimeRenderClear(&pApp->runtime,PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
	PX_ObjectRender(pRenderSurface,pApp->root, elapsed);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_ApplicationEventDefault(&pApp->runtime, e);
	PX_ObjectPostEvent(pApp->root, e);

}

