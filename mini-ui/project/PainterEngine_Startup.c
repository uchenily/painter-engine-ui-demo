#include "PainterEngine_Startup.h"

//////////////////////////////////////////////////////////////////////////
// Functions
static px_byte PX_ApplicationRuntime[PX_APPLICATION_MEMORYPOOL_UI_SIZE
                                     + PX_APPLICATION_MEMORYPOOL_RESOURCES_SIZE
                                     + PX_APPLICATION_MEMORYPOOL_GAME_SIZE
                                     + PX_APPLICATION_MEMORYPOOL_SPACE_SIZE];
px_bool PX_ApplicationInitializeDefault(PX_Runtime *runtime,
                                        px_int screen_width,
                                        px_int screen_height) {
    px_int surface_width = 900, surface_height = 506;
    px_int window_width = 900, window_height = 506;

    if (!PX_RuntimeInitialize(runtime,
                              surface_width,
                              surface_height,
                              window_width,
                              window_height,
                              PX_ApplicationRuntime,
                              sizeof(PX_ApplicationRuntime),
                              PX_APPLICATION_MEMORYPOOL_UI_SIZE,
                              PX_APPLICATION_MEMORYPOOL_RESOURCES_SIZE,
                              PX_APPLICATION_MEMORYPOOL_GAME_SIZE))
        return PX_FALSE;
    return PX_TRUE;
}
px_void PX_ApplicationEventDefault(PX_Runtime *runtime, PX_Object_Event e) {
    if (e.Event == PX_OBJECT_EVENT_WINDOWRESIZE) {
        PX_RuntimeResize(runtime,
                         runtime->surface_width,
                         runtime->surface_height,
                         (px_int) PX_Object_Event_GetWidth(e),
                         (px_int) PX_Object_Event_GetHeight(e));
        return;
    }
}

#ifdef PAINTERENGIN_FILE_SUPPORT

px_bool PX_LoadTextureFromFile(px_memorypool *mp,
                               px_texture *tex,
                               const px_char path[]) {
    PX_IO_Data io;

    io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (PX_TextureCreateFromMemory(mp, io.buffer, io.size, tex)) {
        PX_FreeIOData(&io);
        return PX_TRUE;
    }
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadStringFromFile(px_memorypool *mp,
                              px_string *_string,
                              const px_char path[]) {
    PX_IO_Data io;

    io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    PX_StringInitialize(mp, _string);
    if (PX_StringCat(_string, (const px_char *) io.buffer)) {
        PX_FreeIOData(&io);
        return PX_TRUE;
    }
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool
PX_LoadShapeFromFile(px_memorypool *mp, px_shape *shape, const px_char path[]) {
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (PX_ShapeCreateFromMemory(mp, io.buffer, io.size, shape)) {
        PX_FreeIOData(&io);
        return PX_TRUE;
    }
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadScriptFromFile(px_memory *code, const px_char path[]) {
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (!PX_MemoryCopy(code, io.buffer, 0, io.size))
        goto _ERROR;

    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadUIFormFile(px_memorypool *mp,
                          PX_Object *proot,
                          PX_FontModule *fm,
                          const px_char path[]) {
    px_bool ret = PX_FALSE;
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (PX_Object_DesignerImportToUIObject(
            mp, proot, (const px_char *) io.buffer, fm))
        ret = PX_TRUE;
    PX_FreeIOData(&io);
    return PX_TRUE;
}

px_bool PX_LoadScriptInstanceFromFile(px_memorypool *mp,
                                      PX_VM *ins,
                                      const px_char path[]) {
    PX_IO_Data io = PX_LoadFileToIOData(path);
    if (!io.size) {
        return PX_FALSE;
    }
    if (!PX_VMInitialize(ins, mp, io.buffer, io.size))
        goto _ERROR;

    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadTextureToResource(PX_ResourceLibrary *ResourceLibrary,
                                 const px_char Path[],
                                 const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size)
        goto _ERROR;
    if (!PX_ResourceLibraryLoad(
            ResourceLibrary, PX_RESOURCE_TYPE_TEXTURE, io.buffer, io.size, key))
        goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadStringToResource(PX_ResourceLibrary *ResourceLibrary,
                                const px_char Path[],
                                const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size)
        goto _ERROR;
    if (!PX_ResourceLibraryLoad(
            ResourceLibrary, PX_RESOURCE_TYPE_STRING, io.buffer, io.size, key))
        goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadShapeToResource(PX_ResourceLibrary *ResourceLibrary,
                               const px_char Path[],
                               const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size)
        goto _ERROR;
    if (!PX_ResourceLibraryLoad(
            ResourceLibrary, PX_RESOURCE_TYPE_SHAPE, io.buffer, io.size, key))
        goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadScriptToResource(PX_ResourceLibrary *ResourceLibrary,
                                const px_char Path[],
                                const px_char key[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size)
        goto _ERROR;
    if (!PX_ResourceLibraryLoad(
            ResourceLibrary, PX_RESOURCE_TYPE_SCRIPT, io.buffer, io.size, key))
        goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadFontModuleFromFile(PX_FontModule *fm, const px_char Path[]) {
    PX_IO_Data io;
    io = PX_LoadFileToIOData(Path);
    if (!io.size)
        goto _ERROR;
    if (!PX_FontModuleLoad(fm, io.buffer, io.size))
        goto _ERROR;
    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

px_bool PX_LoadJsonFromFile(PX_Json *json, const px_char *path) {
    PX_IO_Data io = PX_LoadFileToIOData((px_char *) path);
    if (!io.size) {
        return PX_FALSE;
    }

    if (!PX_JsonParse(json, (px_char *) io.buffer))
        goto _ERROR;

    PX_FreeIOData(&io);
    return PX_TRUE;
_ERROR:
    PX_FreeIOData(&io);
    return PX_FALSE;
}

#endif
