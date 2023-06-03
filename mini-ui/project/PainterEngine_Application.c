#include "PainterEngine_Application.h"

#include "kernel/PX_Object.h"
#include "kernel/PX_Object_PushButton.h"
PX_Application App;

PX_Object *root, *button;

px_bool PX_ApplicationInitialize(PX_Application *pApp,
                                 px_int screen_width,
                                 px_int screen_height) {
    PX_ApplicationInitializeDefault(
        &pApp->runtime, screen_width, screen_height);
    root = PX_ObjectCreate(&pApp->runtime.mp_ui, PX_NULL, 0, 0, 0, 0, 0, 0);
    button = PX_Object_PushButtonCreate(
        &pApp->runtime.mp_ui, root, 5, 5, 200, 50, "button", PX_NULL);

    return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp, px_dword elapsed) {}

px_void PX_ApplicationRender(PX_Application *pApp, px_dword elapsed) {
    px_surface *pRenderSurface = &pApp->runtime.RenderSurface;
    PX_RuntimeRenderClear(&pApp->runtime, PX_COLOR(255, 240, 240, 240));
    PX_ObjectRender(pRenderSurface, root, elapsed);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp, PX_Object_Event e) {
    PX_ApplicationEventDefault(&pApp->runtime, e);
    PX_ObjectPostEvent(root, e);
}
