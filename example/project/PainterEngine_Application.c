#if 0
#    include "PainterEngine_Application.h"
PX_Application App;

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);

	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp,px_dword elapsed)
{

}

px_void PX_ApplicationRender(PX_Application *pApp,px_dword elapsed)
{
	px_surface *pRenderSurface=&pApp->runtime.RenderSurface;
	PX_RuntimeRenderClear(&pApp->runtime,PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);

}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_ApplicationEventDefault(&pApp->runtime, e);

	
}
#endif

// www.zhihu.com/question/587721560/answer/3053891651

#include "PainterEngine_Application.h"
PX_Application App;

PX_Object *root, *button_Ok;

px_void
PX_App_OnButtionDown(PX_Object *pObject, PX_Object_Event e, px_void *ptr) {
    printf("user:%s\npassword:%s\n",
           PX_Object_EditGetText(PX_ObjectGetObject(root, ".edit2")),
           PX_Object_EditGetText(PX_ObjectGetObject(root, ".edit3")));
}

px_bool PX_ApplicationInitialize(PX_Application *pApp,
                                 px_int screen_width,
                                 px_int screen_height) {
    PX_ApplicationInitializeDefault(
        &pApp->runtime, screen_width, screen_height);
    root = PX_ObjectCreate(&pApp->runtime.mp_ui, PX_NULL, 0, 0, 0, 0, 0, 0);
    PX_LoadUIFormFile(&pApp->runtime.mp_ui, root, 0, "ui.json");

    button_Ok = PX_ObjectGetObject(root, ".pushbutton0");
    PX_ObjectRegisterEvent(
        button_Ok, PX_OBJECT_EVENT_EXECUTE, PX_App_OnButtionDown, PX_NULL);
    return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp, px_dword elapsed) {}

px_void PX_ApplicationRender(PX_Application *pApp, px_dword elapsed) {
    px_surface *pRenderSurface = &pApp->runtime.RenderSurface;
    PX_RuntimeRenderClear(&pApp->runtime, PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
    PX_ObjectRender(pRenderSurface, root, elapsed);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp, PX_Object_Event e) {
    PX_ApplicationEventDefault(&pApp->runtime, e);
    PX_ObjectPostEvent(root, e);
}
