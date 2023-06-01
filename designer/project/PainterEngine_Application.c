#include "PainterEngine_Application.h"
PX_Application App;

px_byte app_io_cache[1024 * 1024];
px_memorypool app_io_mp;
px_string app_io_export_str;

px_void
PX_ApplicationOnDesignerOpen(px_void *buffer, px_int size, px_void *ptr) {
    PX_Application *pApp = (PX_Application *) ptr;
    px_char *content = (px_char *) buffer;
    PX_Object_DesignerClear(pApp->designer);
    PX_Object_DesignerImport(pApp->designer, content);
}

px_void
PX_ApplicationOnOpenDown(PX_Object *pObject, PX_Object_Event e, px_void *ptr) {
    PX_memset(app_io_cache, 0, sizeof(app_io_cache));
    PX_RequestData("open",
                   app_io_cache,
                   sizeof(app_io_cache),
                   ptr,
                   PX_ApplicationOnDesignerOpen);
}

px_void
PX_ApplicationOnSaveDown(PX_Object *pObject, PX_Object_Event e, px_void *ptr) {
    PX_Application *pApp = (PX_Application *) ptr;
    PX_memset(app_io_cache, 0, sizeof(app_io_cache));
    app_io_mp = MP_Create(app_io_cache, sizeof(app_io_cache));
    PX_StringInitialize(&app_io_mp, &app_io_export_str);
    PX_Object_DesignerExport(pApp->designer, &app_io_export_str);
    PX_RequestData("download:ui.json",
                   app_io_export_str.buffer,
                   app_io_export_str.exreg_strlen + 1,
                   ptr,
                   0);
}

px_bool PX_ApplicationInitialize(PX_Application *pApp,
                                 px_int screen_width,
                                 px_int screen_height) {
    PX_ApplicationInitializeDefault(
        &pApp->runtime, screen_width, screen_height);
    pApp->root
        = PX_ObjectCreate(&pApp->runtime.mp_ui, PX_NULL, 0, 0, 0, 0, 0, 0);
    pApp->ui
        = PX_ObjectCreate(&pApp->runtime.mp_ui, pApp->root, 0, 0, 0, 0, 0, 0);
    pApp->designer = PX_Object_DesignerCreate(&pApp->runtime.mp_ui,
                                              &pApp->runtime.mp_ui,
                                              0,
                                              pApp->root,
                                              pApp->ui,
                                              0,
                                              0,
                                              0);
    PX_Object_DesignerEnable(pApp->designer);
    PX_Object_DesignerDefaultInstall(
        &PX_ObjectGetDesc(PX_Object_Designer, pApp->designer)->ObjectDesc);
    PX_ObjectRegisterEvent(
        pApp->designer, PX_OBJECT_EVENT_OPEN, PX_ApplicationOnOpenDown, pApp);
    PX_ObjectRegisterEvent(
        pApp->designer, PX_OBJECT_EVENT_SAVE, PX_ApplicationOnSaveDown, pApp);
    return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp, px_dword elapsed) {}

px_void PX_ApplicationRender(PX_Application *pApp, px_dword elapsed) {
    px_surface *pRenderSurface = &pApp->runtime.RenderSurface;
    PX_RuntimeRenderClear(&pApp->runtime, PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
    PX_ObjectRender(pRenderSurface, pApp->root, elapsed);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp, PX_Object_Event e) {
    PX_ApplicationEventDefault(&pApp->runtime, e);
    PX_ObjectPostEvent(pApp->root, e);
}

