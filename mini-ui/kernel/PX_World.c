#include "PX_World.h"

px_bool PX_WorldInitialize(px_memorypool *mp,
                           PX_World *World,
                           px_int world_width,
                           px_int world_height,
                           px_int surface_width,
                           px_int surface_height,
                           px_dword calcSize) {
    px_void *ptr;
    PX_memset(World, 0, sizeof(PX_World));
    if (!PX_VectorInitialize(mp, &World->pObjects, sizeof(PX_WorldObject), 256))
        return PX_FALSE;
    if (!PX_VectorInitialize(mp, &World->pNewObjects, sizeof(PX_Object *), 128))
        return PX_FALSE;
    World->mp = mp;
    World->world_height = world_height;
    World->world_width = world_width;
    World->surface_height = surface_height;
    World->surface_width = surface_width;
    World->auxiliaryline = PX_TRUE;
    World->auxiliaryline_color = PX_COLOR(64, 255, 192, 255);
    World->camera_offset = PX_POINT(0, 0, 0);
    World->aliveCount = 0;
    World->offsetx = 0;
    World->offsety = 0;
    PX_WorldSetAuxiliaryXYSpacer(World, 32, 32);
    if (calcSize) {
        ptr = MP_Malloc(World->mp, calcSize);
        if (!ptr) {
            return PX_FALSE;
        }
        World->mp_WorldCalc = MP_Create(ptr, calcSize);
    }
    return PX_TRUE;
}

px_void PX_WorldSetBackwardTexture(PX_World *pWorld, px_texture *ptexture) {
    pWorld->pbackgroundtexture = ptexture;
}

px_void PX_WorldSetFrontwardTexture(PX_World *pWorld, px_texture *ptexture) {
    pWorld->pfrontwardtexture = ptexture;
}

px_void PX_WorldSetAuxiliaryXYSpacer(PX_World *pw, px_int x, px_int y) {
    pw->auxiliaryXSpacer = x;
    pw->auxiliaryYSpacer = y;
}

px_void PX_WorldEnableAuxiliaryLine(PX_World *pw, px_bool bline) {
    pw->auxiliaryline = bline;
}

px_int PX_WorldGetCount(PX_World *World) { return World->pObjects.size; }

px_int PX_WorldGetAliveCount(PX_World *World) {
    px_int i;
    px_int count = 0;
    for (i = 0; i < World->pObjects.size; i++) {
        if (PX_WorldGetWorldObject(World, i)->pObject
            && !PX_WorldGetWorldObject(World, i)->DeleteMark)
            count++;
    }
    return count;
}

PX_WorldObject *PX_WorldGetWorldObject(PX_World *World, px_int index) {
    PX_WorldObject *pwo;
    if (index < 0) {
        PX_ASSERT();
    }
    if (index >= World->pObjects.size) {
        return PX_NULL;
    }
    pwo = PX_VECTORAT(PX_WorldObject, &World->pObjects, index);
    return pwo;
}

PX_Object *PX_WorldGetObject(PX_World *World, px_int index) {
    PX_WorldObject *pwo;
    if (index < 0) {
        PX_ASSERT();
    }
    if (index >= World->pObjects.size) {
        return PX_NULL;
    }
    pwo = PX_VECTORAT(PX_WorldObject, &World->pObjects, index);
    if (pwo->DeleteMark) {
        return PX_NULL;
    }
    return pwo->pObject;
}

px_int PX_WorldAddObjectEx(PX_World *World, PX_Object *pObject) {
    PX_WorldObject *pwo, wo;
    px_int i;
    wo.pObject = pObject;
    wo.DeleteMark = PX_FALSE;

    for (i = 0; i < World->pObjects.size; i++) {
        pwo = PX_VECTORAT(PX_WorldObject, &World->pObjects, i);
        if (!pwo->pObject) {
            *pwo = wo;
            World->aliveCount++;
            return i;
        }
    }
    if (PX_VectorPushback(&World->pObjects, &wo)) {
        World->aliveCount++;
        return World->pObjects.size - 1;
    }
    return -1;
}

px_void PX_WorldRemoveObjectEx(PX_World *world, px_int i_index) {
    PX_WorldObject *pwo
        = PX_VECTORAT(PX_WorldObject, &world->pObjects, i_index);
    if (!pwo) {
        return;
    }
    if (pwo->pObject) {
        pwo->DeleteMark = PX_FALSE;
        world->aliveCount--;
        PX_ObjectDelete(pwo->pObject);
        pwo->pObject = PX_NULL;
    }
}

px_void PX_WorldClear(PX_World *world) {
    px_int i;
    for (i = 0; i < world->pObjects.size; i++) {
        PX_WorldRemoveObjectEx(world, i);
    }
    PX_VectorClear(&world->pObjects);
}

px_void PX_WorldUpdateOffset(PX_World *pw) {
    px_int surface_width, surface_height;
    px_int LeftTopX, LeftTopY;

    surface_width = pw->surface_width;
    surface_height = pw->surface_height;
    LeftTopX = (px_int) (pw->camera_offset.x - surface_width / 2),
    LeftTopY = (px_int) (pw->camera_offset.y - surface_height / 2);

    if (LeftTopX + surface_width > pw->world_width) {
        LeftTopX = pw->world_width - surface_width;
    }

    if (LeftTopY + surface_height > pw->world_height) {
        LeftTopY = pw->world_height - surface_height;
    }

    if (LeftTopX < 0) {
        LeftTopX = 0;
    }
    if (LeftTopY < 0) {
        LeftTopY = 0;
    }

    pw->offsetx = (px_float) LeftTopX;
    pw->offsety = (px_float) LeftTopY;
}

static px_void PX_WorldClildRender(PX_World *pworld,
                                   px_surface *pSurface,
                                   PX_Object *Object,
                                   px_uint elapsed,
                                   px_float oftX,
                                   px_float oftY) {
    if (Object == PX_NULL) {
        return;
    }
    if (Object->Visible == PX_FALSE) {
        return;
    }
    if (Object->Func_ObjectRender != 0) {
        px_float x = Object->x;
        px_float y = Object->y;
        Object->x -= oftX;
        Object->y -= oftY;
        Object->Func_ObjectRender(pSurface, Object, elapsed);

        if (pworld->showImpactRegion) {
            if (Object->diameter) {
                PX_GeoDrawSolidCircle(pSurface,
                                      (px_int) Object->x,
                                      (px_int) Object->y,
                                      (px_int) Object->diameter / 2,
                                      PX_COLOR(32, 255, 0, 0));
            } else {
                PX_GeoDrawRect(pSurface,
                               (px_int) (Object->x - Object->Width / 2),
                               (px_int) (Object->y - Object->Height / 2),
                               (px_int) (Object->x + Object->Width / 2),
                               (px_int) (Object->y + Object->Height / 2),
                               PX_COLOR(32, 255, 0, 0));
            }
        }

        Object->x = x;
        Object->y = y;
    }
    if (Object->pNextBrother != PX_NULL) {
        PX_WorldClildRender(
            pworld, pSurface, Object->pNextBrother, elapsed, oftX, oftY);
    }
    if (Object->pChilds != PX_NULL) {
        PX_WorldClildRender(
            pworld, pSurface, Object->pChilds, elapsed, oftX, oftY);
    }
}

px_void PX_WorldRender(px_surface *psurface, PX_World *pw, px_dword elapsed) {
    px_int i, j;
    px_int surface_width, surface_height;
    PX_QuickSortAtom *ArrayIndex;
    px_memorypool *calcmp;
    px_int sx, sy;
    PX_WorldObject *pwo;

    if (pw == PX_NULL) {
        return;
    }

    calcmp = &pw->mp_WorldCalc;
    MP_Reset(calcmp);

    PX_WorldUpdateOffset(pw);

    surface_width = pw->surface_width;
    surface_height = pw->surface_height;

    if (pw->pbackgroundtexture) {
        px_float xfactor;
        px_float yfactor;
        px_int x;
        px_int y;
        if (pw->world_width != pw->surface_width) {
            xfactor
                = pw->offsetx * 1.0f / (pw->world_width - pw->surface_width);
        } else {
            xfactor = 0;
        }

        if (pw->world_height != pw->surface_height) {
            yfactor
                = pw->offsety * 1.0f / (pw->world_height - pw->surface_height);
        } else {
            yfactor = 0;
        }

        x = (px_int) ((pw->pbackgroundtexture->width - pw->surface_width)
                      * xfactor);
        y = (px_int) ((pw->pbackgroundtexture->height - pw->surface_height)
                      * yfactor);
        PX_TextureRender(
            psurface, pw->pbackgroundtexture, -x, -y, PX_ALIGN_LEFTTOP, 0);
    }

    if (pw->auxiliaryline && pw->auxiliaryline_color._argb.a) {
        for (sy = pw->auxiliaryYSpacer
                  - ((px_int) pw->offsety % pw->auxiliaryYSpacer);
             sy < surface_height;
             sy += pw->auxiliaryYSpacer) {
            PX_GeoDrawLine(psurface,
                           0,
                           sy,
                           surface_width - 1,
                           sy,
                           1,
                           pw->auxiliaryline_color);
        }

        for (sx = pw->auxiliaryXSpacer
                  - ((px_int) pw->offsetx % pw->auxiliaryXSpacer);
             sx < surface_width;
             sx += pw->auxiliaryXSpacer) {
            PX_GeoDrawLine(psurface,
                           sx,
                           0,
                           sx,
                           surface_height - 1,
                           1,
                           pw->auxiliaryline_color);
        }
    }

    ArrayIndex = (PX_QuickSortAtom *) MP_Malloc(
        calcmp, pw->aliveCount * sizeof(PX_QuickSortAtom));
    if (pw->aliveCount && !ArrayIndex) {
        PX_ERROR("out of memory");
    }
    if (ArrayIndex) {
        j = 0;

        for (i = 0; i < pw->pObjects.size; i++) {
            pwo = PX_VECTORAT(PX_WorldObject, &pw->pObjects, i);
            if (!pwo->pObject) {
                continue;
            }
            ArrayIndex[j].weight = pwo->pObject->z;
            ArrayIndex[j].pData = pwo->pObject;
            j++;
        }

        PX_Quicksort_MaxToMin(ArrayIndex, 0, pw->aliveCount - 1);

        for (i = 0; i < pw->aliveCount; i++) {
            PX_WorldClildRender(pw,
                                psurface,
                                (PX_Object *) ArrayIndex[i].pData,
                                elapsed,
                                pw->offsetx,
                                pw->offsety);
        }
    }
    MP_Reset(calcmp);

    if (pw->pfrontwardtexture) {
        px_float xfactor;
        px_float yfactor;
        px_int x;
        px_int y;
        if (pw->world_width != pw->surface_width) {
            xfactor
                = pw->offsetx * 1.0f / (pw->world_width - pw->surface_width);
        } else {
            xfactor = 0;
        }

        if (pw->world_height != pw->surface_height) {
            yfactor
                = pw->offsety * 1.0f / (pw->world_height - pw->surface_height);
        } else {
            yfactor = 0;
        }

        x = (px_int) ((pw->pfrontwardtexture->width - pw->surface_width)
                      * xfactor);
        y = (px_int) ((pw->pfrontwardtexture->height - pw->surface_height)
                      * yfactor);
        PX_TextureRender(
            psurface, pw->pfrontwardtexture, -x, -y, PX_ALIGN_LEFTTOP, 0);
    }
}

px_void PX_WorldSetImpact(PX_Object *pObj, px_dword type, px_dword Intersect) {
    pObj->impact_object_type = type;
    pObj->impact_target_type = Intersect;
}

px_bool PX_WorldAddObject(PX_World *World, PX_Object *pObject) {
    return PX_VectorPushback(&World->pNewObjects, &pObject);
}

px_void PX_WorldSetAuxiliaryLineColor(PX_World *pw, px_color color) {
    pw->auxiliaryline_color = color;
}

px_point PX_WorldObjectXYtoScreenXY(PX_World *pw, px_float x, px_float y) {
    return PX_POINT(x - pw->offsetx, y - pw->offsety, 0);
}

px_void PX_WorldPostEvent(PX_World *pw, PX_Object_Event e) {
    PX_WorldObject *pwo;
    px_int i;
    for (i = 0; i < pw->pObjects.size; i++) {
        pwo = PX_VECTORAT(PX_WorldObject, &pw->pObjects, i);
        if (pwo->pObject) {
            PX_ObjectPostEvent(pwo->pObject, e);
        }
    }
}

px_void PX_WorldFree(PX_World *pw) {
    PX_WorldObject *pwo;
    px_int i;
    for (i = 0; i < pw->pObjects.size; i++) {
        pwo = PX_VECTORAT(PX_WorldObject, &pw->pObjects, i);
        if (pwo->pObject) {
            PX_ObjectDelete(pwo->pObject);
        }
    }

    for (i = 0; i < pw->pNewObjects.size; i++) {
        PX_Object *pObject = *PX_VECTORAT(PX_Object *, &pw->pObjects, i);
        if (pObject) {
            PX_ObjectDelete(pObject);
        }
    }

    PX_VectorFree(&pw->pObjects);
    PX_VectorFree(&pw->pNewObjects);

    MP_Free(pw->mp, pw->mp_WorldCalc.StartAddr);
}

px_void PX_WorldSetCamera(PX_World *World, px_point camera_center_point) {
    World->camera_offset = camera_center_point;
    PX_WorldUpdateOffset(World);
}

px_void PX_WorldRemoveObject(PX_World *world, PX_Object *pObject) {
    PX_WorldObject *pwo;
    if (pObject->world_index != -1
        && pObject->world_index < world->pObjects.size) {
        pwo = PX_VECTORAT(
            PX_WorldObject, &world->pObjects, pObject->world_index);
        if (pwo->pObject == pObject) {
            pwo->DeleteMark = PX_TRUE;
            return;
        }
    }
}

px_void PX_WorldRemoveObjectByIndex(PX_World *world, px_int i_index) {
    PX_WorldObject *pwo
        = PX_VECTORAT(PX_WorldObject, &world->pObjects, i_index);

#ifdef PX_DEBUG_MODE
    if (i_index > world->pObjects.size) {
        PX_ASSERT();
    }
#endif

    if (pwo->pObject) {
        pwo->DeleteMark = PX_TRUE;
    }
}

px_void
PX_WorldSetSize(PX_World *pWorld, px_int world_width, px_int world_height) {
    pWorld->world_width = world_width;
    pWorld->world_height = world_height;
}
