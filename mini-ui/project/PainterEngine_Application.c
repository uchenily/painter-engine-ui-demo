#include "PainterEngine_Application.h"

#include "kernel/PX_Object.h"
#include "kernel/PX_Object_Menu.h"
#include "kernel/PX_Object_PushButton.h"
PX_Application App;

PX_Object *menu;
PX_Object *root, *button;

#include <stdio.h>
px_void PX_ApplicationOnPushButton(PX_Object *pObject,
                                   PX_Object_Event e,
                                   px_void *ptr) {
    printf("hello world\n");
}

px_bool PX_ApplicationInitialize(PX_Application *pApp,
                                 px_int screen_width,
                                 px_int screen_height) {
    PX_ApplicationInitializeDefault(
        &pApp->runtime, screen_width, screen_height);
    menu = PX_Object_MenuCreate(
        &pApp->runtime.mp_ui, PX_NULL, 0, 0, 64, PX_NULL);
    PX_Object_Menu_Item *pItem, *pItem2;
    pItem = PX_Object_MenuAddItem(
        menu, PX_Object_MenuGetRootItem(menu), "File", PX_NULL, PX_NULL);
    pItem2 = PX_Object_MenuAddItem(menu, pItem, "New", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem2, "File", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem2, "Directory", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem, "New", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem, "Open", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem, "Close", PX_NULL, PX_NULL);

    pItem = PX_Object_MenuAddItem(
        menu, PX_Object_MenuGetRootItem(menu), "Edit", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem, "Copy", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem, "Paste", PX_NULL, PX_NULL);
    PX_Object_MenuAddItem(menu, pItem, "Find", PX_NULL, PX_NULL);

    pItem = PX_Object_MenuAddItem(
        menu, PX_Object_MenuGetRootItem(menu), "Help", PX_NULL, PX_NULL);

    root = PX_ObjectCreate(&pApp->runtime.mp_ui, PX_NULL, 0, 0, 0, 0, 0, 0);
    button = PX_Object_PushButtonCreate(
        &pApp->runtime.mp_ui, root, 100, 300, 200, 50, "button", PX_NULL);
    PX_ObjectRegisterEvent(
        button, PX_OBJECT_EVENT_EXECUTE, PX_ApplicationOnPushButton, pApp);

    return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp, px_dword elapsed) {}

px_void PX_ApplicationRender(PX_Application *pApp, px_dword elapsed) {
    px_surface *pRenderSurface = &pApp->runtime.RenderSurface;
    PX_RuntimeRenderClear(&pApp->runtime, PX_COLOR(255, 240, 240, 240));
    PX_ObjectRender(pRenderSurface, menu, elapsed);
    PX_ObjectRender(pRenderSurface, root, elapsed);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp, PX_Object_Event e) {
    PX_ApplicationEventDefault(&pApp->runtime, e);
    PX_ObjectPostEvent(menu, e);
    PX_ObjectPostEvent(root, e);
}
