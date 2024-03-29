#include "PX_Object_Canvas.h"

px_void PX_Object_CanvasOnRender(px_surface* psurface, PX_Object* pObject, px_dword elapsed)
{
	px_float size;
	px_float x, y, w, h;
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_OBJECT_INHERIT_CODE(pObject, x, y, w, h);

	size = PX_Object_PainterBoxGetToolSize(pdesc->pPainterBoxObject);
	PX_SurfaceRender(psurface, &pdesc->view_surface, (px_int)x, (px_int)y, PX_ALIGN_LEFTTOP,0);
	PX_GeoDrawBorder(psurface, (px_int)x, (px_int)y, (px_int)(x + w), (px_int)(y + h), 1, PX_COLOR_BLACK);
	PX_SurfaceSetLimit(psurface, (px_int)x, (px_int)y, (px_int)(x + w), (px_int)(h + y));
	PX_GeoDrawCircle(psurface, (px_int)(x+pdesc->reg_x), (px_int)(y+pdesc->reg_y),(px_int)(size * pdesc->scale), 1, PX_COLOR(255, 0, 0, 0));
	PX_SurfaceUnlimit(psurface);
}

px_void PX_Object_CanvasOnFree(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_TextureFree(&pdesc->cache_backward_surface);
	PX_TextureFree(&pdesc->cache_editing_surface);
	PX_TextureFree(&pdesc->cache_frontward_surface);
	PX_TextureFree(&pdesc->cache_surface);
	PX_TextureFree(&pdesc->view_surface);
}

px_surface *PX_Object_CanvasGetCurrentEditingSurface(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_Object_LayerBox* pLayerBox = PX_ObjectGetDesc(PX_Object_LayerBox, pdesc->pLayerBoxObject);
	return PX_Object_LayerBoxGetEditingSurface(pdesc->pLayerBoxObject);
}

px_void PX_Object_CanvasRenderLayerToSurface(PX_Object_LayerBox* pLayerBox, px_int index, px_texture* prendertarget)
{
	if (pLayerBox->layers[index].attribute == PX_OBJECT_LAYER_ATTRIBUTE_CLIP)
	{
		//find normal
		px_int j;
		for (j = index + 1; j <= PX_COUNTOF(pLayerBox->layers); j++)
		{
			//find mask
			if (pLayerBox->layers[j].attribute == PX_OBJECT_LAYER_ATTRIBUTE_NORMAL)
			{
				PX_TextureRenderMask(prendertarget, &pLayerBox->layers[j].surface_layer, &pLayerBox->layers[index].surface_layer, 0, 0, PX_ALIGN_LEFTTOP, 0);
			}
		}
	}
	else
	{
		PX_TextureRender(prendertarget, &pLayerBox->layers[index].surface_layer, 0, 0, PX_ALIGN_LEFTTOP, 0);
	}
}

px_void PX_Object_CanvasRenderBackFrontLayersToCache(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_Object_LayerBox* pLayerBox = PX_ObjectGetDesc(PX_Object_LayerBox, pdesc->pLayerBoxObject);
	px_texture* prender;
	px_int index = 0;

	PX_SurfaceClearAll(&pdesc->cache_frontward_surface, PX_COLOR(0, 0, 0, 0));
	PX_SurfaceClearAll(&pdesc->cache_backward_surface, PX_COLOR(255, 255, 255, 255));

	while (pLayerBox->layers[index].activating)
	{
		index++;
	}

	prender = &pdesc->cache_backward_surface;
	while (index >= 0)
	{
		if (pLayerBox->layers[index].activating)
		{
			if (pLayerBox->layers[index].editing)
			{
				prender = &pdesc->cache_frontward_surface;
			}
			else
			{
				PX_Object_CanvasRenderLayerToSurface(pLayerBox, index, prender);
			}
		}
		index--;
	}

}

px_void PX_Object_CanvasRenderEditingLayersToCache(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_Object_LayerBox* pLayerBox = PX_ObjectGetDesc(PX_Object_LayerBox, pdesc->pLayerBoxObject);
	px_int index = 0;
	px_int activatingIndex = 0;
	PX_SurfaceClearAll(&pdesc->cache_editing_surface, PX_COLOR(0, 0, 0, 0));

	for (activatingIndex = 0; activatingIndex < PX_COUNTOF(pLayerBox->layers); activatingIndex++)
	{
		if (pLayerBox->layers[activatingIndex].activating == PX_FALSE)
		{
			break;
		}
		if (pLayerBox->layers[activatingIndex].editing == PX_TRUE)
		{
			PX_Object_CanvasRenderLayerToSurface(pLayerBox, activatingIndex, &pdesc->cache_editing_surface);
			break;
		}
	}
}
px_void PX_Object_CanvasRenderCombineCache(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_TextureCover(&pdesc->cache_surface, &pdesc->cache_backward_surface, 0, 0, PX_ALIGN_LEFTTOP);
	PX_TextureRender(&pdesc->cache_surface, &pdesc->cache_editing_surface, 0, 0, PX_ALIGN_LEFTTOP,0);
	PX_TextureRender(&pdesc->cache_surface, &pdesc->cache_frontward_surface, 0, 0, PX_ALIGN_LEFTTOP, 0);
}

px_void PX_Object_CanvasRenderCacheToView(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	px_int ltx, lty, rbx, rby;
	px_int v_width = pdesc->view_surface.width;
	px_int v_height = pdesc->view_surface.height;
	px_int clipw, cliph;
	px_texture clip_tex;

	clipw = (px_int)(v_width / pdesc->scale);
	cliph = (px_int)(v_height / pdesc->scale);

	ltx = (px_int)(pdesc->view_x - clipw / 2);
	rbx = (px_int)(pdesc->view_x + clipw / 2);
	lty = (px_int)(pdesc->view_y - cliph / 2);
	rby = (px_int)(pdesc->view_y + cliph / 2);

	if (!PX_TextureCreate(pdesc->mp, &clip_tex, clipw, cliph))
	{
		return;
	}

	PX_SurfaceClearAll(&clip_tex, PX_COLOR(0, 0, 0, 0));

	PX_TextureRender(&clip_tex, &pdesc->cache_backward_surface, -ltx, -lty, PX_ALIGN_LEFTTOP, 0);
	PX_TextureRender(&clip_tex, &pdesc->cache_editing_surface, -ltx, -lty, PX_ALIGN_LEFTTOP, 0);
	PX_TextureRender(&clip_tex, &pdesc->cache_frontward_surface, -ltx, -lty, PX_ALIGN_LEFTTOP, 0);

	if (!PX_TextureScaleToTexture(&clip_tex, &pdesc->view_surface))
	{
		PX_TextureFree(&clip_tex);
		return;
	}
	PX_TextureFree(&clip_tex);
}

px_point2D PX_Object_CanvasCanvasPostionToViewPosition(PX_Object* pObject, px_point2D canvasPos)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	px_float sw, sh;
	px_point2D p;
	//distance
	sw = pdesc->view_surface.width*1.0f / pdesc->scale;
	sh = pdesc->view_surface.height * 1.0f / pdesc->scale;
	p = PX_Point2DSub(canvasPos, PX_POINT2D(pdesc->view_x, pdesc->view_y));
	p.x = (p.x / sw*2 ) * pdesc->view_surface.width/2 + pdesc->view_surface.width / 2;
	p.y = (p.y / sh*2 ) * pdesc->view_surface.height/2+ pdesc->view_surface.height / 2;
	return p;
}

px_point2D PX_Object_CanvasViewPositionToCanvasPostion(PX_Object* pObject, px_point2D ViewPos)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	px_float sw, sh;
	px_point2D p;
	//distance
	sw = pdesc->view_surface.width*1.f;
	sh = pdesc->view_surface.height * 1.f;
	p= PX_Point2DSub(ViewPos, PX_POINT2D(sw/2, sh/2));//distance
	
	p.x /= sw/2; //rate
	p.y /= sh / 2;

	p.x *= sw / pdesc->scale/2;//relative position
	p.y*= sh / pdesc->scale / 2;
	p.x += pdesc->view_x;//absolute position
	p.y += pdesc->view_y;
	return p;
}

px_void PX_Object_CanvasUpdateSliderBar(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	px_float w = pdesc->cache_editing_surface.width*1.f;
	px_float h = pdesc->cache_editing_surface.height * 1.f;
	px_float vw = pdesc->view_surface.width / pdesc->scale;
	px_float vh = pdesc->view_surface.height / pdesc->scale;
	px_float range_min_x = 0;
	px_float range_max_x = pdesc->cache_editing_surface.width * 1.f;
	px_float range_min_y =  0;
	px_float range_max_y = pdesc->cache_editing_surface.height * 1.f;
	px_float len_x = vw / w * pdesc->pHSliderBar->Width;
	px_float len_y = vh / h * pdesc->pVSliderBar->Height;
	px_float valuex = pdesc->view_x;
	px_float valuey = pdesc->view_y;

	if (len_x >= pdesc->pHSliderBar->Width/1.5f)
	{
		len_x = pdesc->pHSliderBar->Width/1.5f;
	}
	if (len_y >= pdesc->pVSliderBar->Height/1.5f)
	{
		len_y = pdesc->pVSliderBar->Height/1.5f;
	}


	PX_Object_SliderBarSetRange(pdesc->pHSliderBar, (px_int)range_min_x, (px_int)range_max_x);
	PX_Object_SliderBarSetSliderButtonLength(pdesc->pHSliderBar, (px_int)len_x);
	PX_Object_SliderBarSetValue(pdesc->pHSliderBar, (px_int)valuex);
	
	
	PX_Object_SliderBarSetRange(pdesc->pVSliderBar, (px_int)range_min_y, (px_int)range_max_y);
	PX_Object_SliderBarSetSliderButtonLength(pdesc->pVSliderBar, (px_int)len_y);
	PX_Object_SliderBarSetValue(pdesc->pVSliderBar, (px_int)valuey);
	
}

px_void PX_Object_CanvasRenderToLayerPreview(PX_Object* pObject)
{
	px_float scale;
	px_int neww, newh,ltoftx,ltofty;
	px_float vcx, vcy, vcw, vch;
	px_texture mintex;
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	px_texture* ptexture = PX_Object_LayerBoxGetPreviewSurface(pdesc->pLayerBoxObject);

	if (pdesc->cache_editing_surface.width>pdesc->cache_editing_surface.height)
	{
		scale = 120.f / pdesc->cache_editing_surface.width;
	}
	else
	{
		scale = 120.f / pdesc->cache_editing_surface.height;
	}
	neww = (px_int)(pdesc->cache_editing_surface.width * scale);
	newh = (px_int)(pdesc->cache_editing_surface.height* scale);




	if (!PX_TextureCreateScale(pdesc->mp,&pdesc->cache_surface,neww,newh,&mintex))
	{
		return;
	}
	PX_SurfaceClearAll(ptexture, PX_COLOR(0, 0, 0, 0));
	PX_TextureRender(ptexture, &mintex, ptexture->width / 2, ptexture->height / 2, PX_ALIGN_CENTER, 0);
	ltoftx = ptexture->width / 2 - mintex.width / 2;
	ltofty= ptexture->height / 2 - mintex.height / 2;

	vcx = pdesc->view_x * 1.0f / pdesc->cache_editing_surface.width * neww;
	vcy = pdesc->view_y * 1.0f / pdesc->cache_editing_surface.height * newh;
	vcw = pdesc->view_surface.width / pdesc->scale / pdesc->cache_editing_surface.width * neww;
	vch = pdesc->view_surface.height*1.0f / pdesc->cache_editing_surface.height/pdesc->scale * newh;

	PX_GeoDrawBorder(ptexture, (px_int)(ltoftx+vcx - vcw / 2), (px_int)(ltofty+vcy - vch / 2), (px_int)(ltoftx + vcx + vcw / 2), (px_int)(ltofty + vcy + vch / 2), 1, PX_COLOR_BLACK);

	PX_TextureFree(&mintex);

}

px_void PX_Object_CanvasOnCursorWheeling(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	px_float cx, cy, cz;
	px_float x, y,w,h;

	PX_OBJECT_INHERIT_CODE(pObject, x, y, w, h);
	if (PX_ObjectIsCursorInRegion(pObject,e))
	{
		px_float scale,new_v_x,new_v_y;
		px_point2D p2d;
		cx = PX_Object_Event_GetCursorX(e)-x;
		cy = PX_Object_Event_GetCursorY(e)-y;
		cz = PX_Object_Event_GetCursorZ(e);
		
		if (cz > 0)
		{
			scale = 1.1f;
			new_v_x = cx - cx / scale + pdesc->view_surface.width / scale / 2;
			new_v_y = cy - cy / scale + pdesc->view_surface.height / scale / 2;
			p2d = PX_Object_CanvasViewPositionToCanvasPostion(pObject, PX_POINT2D(new_v_x, new_v_y));

			pdesc->view_x = p2d.x;
			pdesc->view_y = p2d.y;
		}
		else
		{
			scale = 0.9f;
		}
		

		pdesc->scale *= scale;
		
		if (pdesc->scale<pdesc->min_scale)
		{
			pdesc->scale = pdesc->min_scale;
		}
		if (pdesc->scale>pdesc->max_scale)
		{
			pdesc->scale = pdesc->max_scale;
		}

		if (pdesc->view_x< 0)
		{
			pdesc->view_x =0;
		};

		if (pdesc->view_x > pdesc->cache_editing_surface.width )
		{
			pdesc->view_x = pdesc->cache_editing_surface.width * 1.f;
		};

		if (pdesc->view_y < 0)
		{
			pdesc->view_y=0;
		};
		if (pdesc->view_y > pdesc->cache_editing_surface.height )
		{
			pdesc->view_y = pdesc->cache_editing_surface.height * 1.f;
		};
		PX_Object_CanvasRenderCacheToView(pObject);
		PX_Object_CanvasUpdateSliderBar(pObject);
		PX_Object_CanvasRenderCombineCache(pObject);
		PX_Object_CanvasRenderToLayerPreview(pObject);
	}
	
}

px_void PX_Object_CanvasOnHSliderBarChanged(PX_Object* pObject, PX_Object_Event  e, px_void* ptr)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, (PX_Object*)ptr);
	pdesc->view_x = PX_Object_SliderBarGetValue(pObject) * 1.f;
	if (pdesc->view_x < 0)
	{
		pdesc->view_x = 0;
	}
	if (pdesc->view_x > pdesc->cache_editing_surface.width)
	{
		pdesc->view_x = pdesc->view_surface.width * 1.f;
	}
	PX_Object_CanvasRenderCacheToView((PX_Object*)ptr);
	PX_Object_CanvasRenderToLayerPreview((PX_Object*)ptr);
}

px_void PX_Object_CanvasOnVSliderBarChanged(PX_Object* pObject, PX_Object_Event  e, px_void* ptr)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, (PX_Object*)ptr);
	pdesc->view_y = PX_Object_SliderBarGetValue(pObject) * 1.f;
	if (pdesc->view_y < 0)
	{
		pdesc->view_y = 0;
	}
	if (pdesc->view_y > pdesc->cache_editing_surface.height)
	{
		pdesc->view_y = pdesc->view_surface.height * 1.f;
	}
	PX_Object_CanvasRenderCacheToView((PX_Object*)ptr);
	PX_Object_CanvasRenderToLayerPreview((PX_Object*)ptr);
}

px_void PX_Object_CanvasEnterEdit(PX_Object* pObject)
{
	px_float size,smooth;
	px_color clr;
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_Object_PainterBox* pPainterBox = PX_ObjectGetDesc(PX_Object_PainterBox, pdesc->pPainterBoxObject);
	PX_CANVASVM_OPCODE opcode;
	PX_CanvasVM_FilterMode fmode= PX_CanvasVM_FilterMode_None;
	switch (pPainterBox->tool_type)
	{
	case PX_OBJECT_PAINTERBOX_TOOL_TYPE_PEN:
	{
		opcode = PX_CANVASVM_OPCODE_PEN;
		fmode = PX_CanvasVM_FilterMode_BSpline;

	}
	break;
	case PX_OBJECT_PAINTERBOX_TOOL_TYPE_SPRAY:
	{
		opcode = PX_CANVASVM_OPCODE_SPRAY;
		fmode = PX_CanvasVM_FilterMode_BSpline;
	}
	break;
	case PX_OBJECT_PAINTERBOX_TOOL_TYPE_ERASER:
	{
		opcode = PX_CANVASVM_OPCODE_ERASER;
		fmode = PX_CanvasVM_FilterMode_None;
	}
	break;
	case PX_OBJECT_PAINTERBOX_TOOL_TYPE_HAND:
	{
		opcode = PX_CANVASVM_OPCODE_HAND;
		fmode = PX_CanvasVM_FilterMode_None;
	}
	break;
	case PX_OBJECT_PAINTERBOX_TOOL_TYPE_PAINT:
	{
		opcode = PX_CANVASVM_OPCODE_PAINT;
		fmode = PX_CanvasVM_FilterMode_None;
	}
	break;
	case PX_OBJECT_PAINTERBOX_TOOL_TYPE_NONE:
	{
		fmode = PX_CanvasVM_FilterMode_None;
		return;
	}
	break;
	}
	size = PX_Object_PainterBoxGetToolSize(pdesc->pPainterBoxObject);
	clr = PX_Object_PainterBoxGetToolColor(pdesc->pPainterBoxObject);
	smooth = PX_Object_PainterBoxGetToolSmooth(pdesc->pPainterBoxObject);
	pdesc->mode = PX_OBJECT_CANVAS_MODE_EDIT;
	PX_CanvasVMOnBegin(&pdesc->CanvasVM, opcode, size * pdesc->scale, clr, smooth,  0.35f);
	
}

px_void PX_Object_CanvasExitEdit(PX_Object* pObject)
{
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_Object_PainterBox* pPainterBox = PX_ObjectGetDesc(PX_Object_PainterBox, pdesc->pPainterBoxObject);
	PX_CANVASVM_OPCODE opcode;
	PX_CanvasVM_FilterMode fmode;
	switch (pdesc->CanvasVM.tools[pdesc->CanvasVM.reg_current_tool_index].opcode)
	{
	case PX_CANVASVM_OPCODE_ERASER:
	case PX_CANVASVM_OPCODE_HAND:
	case PX_CANVASVM_OPCODE_PAINT:
	break;
	case PX_CANVASVM_OPCODE_PEN:
	case PX_CANVASVM_OPCODE_SPRAY:
	{
		opcode = PX_CANVASVM_OPCODE_HAND;
		fmode = PX_CanvasVM_FilterMode_None;
	}
	break;
	break;
	}
	pdesc->mode = PX_OBJECT_CANVAS_MODE_STANDBY;
	PX_Object_LayerBoxUpdateEditingLayer(pdesc->pLayerBoxObject);
}

px_void PX_Object_CanvasOnCursorDown(PX_Object* pObject, PX_Object_Event  e, px_void* ptr)
{
	px_float x, y, w, h;
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_OBJECT_INHERIT_CODE(pObject, x, y, w, h);

	if (PX_ObjectIsCursorInRegion(pObject,e))
	{
		pdesc->reg_x = PX_Object_Event_GetCursorX(e) - x;
		pdesc->reg_y = PX_Object_Event_GetCursorY(e) - y;
		pdesc->reg_z = 1;
		PX_Object_CanvasEnterEdit(pObject);
	}
}

px_void PX_Object_CanvasOnCursorMove(PX_Object* pObject, PX_Object_Event  e, px_void* ptr)
{
	px_float x, y, w, h;
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_OBJECT_INHERIT_CODE(pObject, x, y, w, h);
	if (PX_ObjectIsCursorInRegion(pObject, e))
	{
		pdesc->reg_x = PX_Object_Event_GetCursorX(e) - x;
		pdesc->reg_y = PX_Object_Event_GetCursorY(e) - y;
		pdesc->reg_z = 1;
	}

	if (pdesc->mode == PX_OBJECT_CANVAS_MODE_EDIT)
	{
		//done
		PX_CanvasVMOnEnd(&pdesc->CanvasVM);
		pdesc->mode = PX_OBJECT_CANVAS_MODE_STANDBY;
	}

}

px_void PX_Object_CanvasOnCursorDrag(PX_Object* pObject, PX_Object_Event  e, px_void* ptr)
{
	px_float x, y, w, h;
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	px_surface* prender = PX_Object_CanvasGetCurrentEditingSurface(pObject);
	if (!prender)
	{
		return;
	}
	PX_OBJECT_INHERIT_CODE(pObject, x, y, w, h);

	if (PX_ObjectIsCursorInRegion(pObject, e))
	{
		pdesc->reg_x = PX_Object_Event_GetCursorX(e) - x;
		pdesc->reg_y = PX_Object_Event_GetCursorY(e) - y;
	}
	
	if (pdesc->mode == PX_OBJECT_CANVAS_MODE_EDIT)
	{
		//editing
		px_point2D canvaspos;
		canvaspos = PX_Object_CanvasViewPositionToCanvasPostion(pObject, PX_POINT2D(pdesc->reg_x, pdesc->reg_y));
		PX_CanvasVMOnMove(&pdesc->CanvasVM, pdesc->reg_x, pdesc->reg_y, pdesc->reg_z);
		PX_CanvasVMPaint(&pdesc->CanvasVM, prender, PX_NULL);
		PX_Object_CanvasRenderEditingLayersToCache(pObject);
		PX_Object_CanvasRenderCacheToView(pObject);

	}
}

px_void PX_Object_CanvasOnCursorUp(PX_Object* pObject, PX_Object_Event  e, px_void* ptr)
{
	px_float x, y, w, h;
	PX_Object_Canvas* pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	PX_OBJECT_INHERIT_CODE(pObject, x, y, w, h);

	if (PX_ObjectIsCursorInRegion(pObject, e))
	{
		pdesc->reg_x = PX_Object_Event_GetCursorX(e) - x;
		pdesc->reg_y = PX_Object_Event_GetCursorY(e) - y;
	}

	if (pdesc->mode == PX_OBJECT_CANVAS_MODE_EDIT)
	{
		//done
		PX_CanvasVMOnEnd(&pdesc->CanvasVM);
		pdesc->mode = PX_OBJECT_CANVAS_MODE_STANDBY;
	}
}



PX_Object* PX_Object_CanvasCreate(px_memorypool* mp, PX_Object* Parent, px_int x, px_int y, px_int width, px_int height, PX_Object* pLayerBoxObject, PX_Object* pPainterBoxObject)
{
	PX_Object_Canvas desc = { 0 }, * pdesc;
	PX_Object* pObject;
	PX_Object_LayerBox* pLayerBox;
	if (width<128)
	{
		width = 128;
	}
	if (height < 128)
	{
		height = 128;
	}

	if (pLayerBoxObject->Type!=PX_OBJECT_TYPE_LAYERBOX)
	{
		PX_ASSERT();
		return PX_NULL;
	}
	if (pPainterBoxObject->Type != PX_OBJECT_TYPE_PAINTERBOX)
	{
		PX_ASSERT();
		return PX_NULL;
	}

	pObject = PX_ObjectCreateEx(mp, Parent, (px_float)x, (px_float)y, 0, (px_float)width, (px_float)height, 0, PX_OBJECT_TYPE_CANVAS, 0, PX_Object_CanvasOnRender, PX_Object_CanvasOnFree, &desc, sizeof(desc));
	pdesc = PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	pdesc->mp = mp;
	pdesc->activating_texture_index = -1;
	pdesc->pPainterBoxObject = pPainterBoxObject;
	pdesc->pLayerBoxObject = pLayerBoxObject;
	pdesc->scale = 1;
	pdesc->max_scale = 10;
	pLayerBox = PX_ObjectGetDesc(PX_Object_LayerBox, pLayerBoxObject);

	pdesc->pHSliderBar = PX_Object_SliderBarCreate(mp, pObject, 0, height - 24, width-24, 24, PX_OBJECT_SLIDERBAR_TYPE_HORIZONTAL, PX_OBJECT_SLIDERBAR_STYLE_BOX);
	pdesc->pVSliderBar = PX_Object_SliderBarCreate(mp, pObject, width-24, 0, 24, height-24, PX_OBJECT_SLIDERBAR_TYPE_VERTICAL, PX_OBJECT_SLIDERBAR_STYLE_BOX);
	PX_Object_SliderBarSetBackgroundColor(pdesc->pHSliderBar, PX_COLOR_WHITE);
	PX_Object_SliderBarSetColor(pdesc->pHSliderBar, PX_COLOR_BLACK);
	
	PX_Object_SliderBarSetBackgroundColor(pdesc->pVSliderBar, PX_COLOR_WHITE);
	PX_Object_SliderBarSetColor(pdesc->pVSliderBar, PX_COLOR_BLACK);

	if (!PX_CanvasVMInitialize(mp, &pdesc->CanvasVM))
		return PX_NULL;

	if (!PX_TextureCreate(mp, &pdesc->cache_backward_surface, pLayerBox->width, pLayerBox->height))
	{
		PX_ASSERT();
		return PX_NULL;
	}
	if (!PX_TextureCreate(mp, &pdesc->cache_frontward_surface, pLayerBox->width, pLayerBox->height))
	{
		PX_ASSERT();
		return PX_NULL;
	}

	if (!PX_TextureCreate(mp, &pdesc->cache_editing_surface, pLayerBox->width, pLayerBox->height))
	{
		PX_ASSERT();
		return PX_NULL;
	}

	if (!PX_TextureCreate(mp, &pdesc->cache_surface, pLayerBox->width, pLayerBox->height))
	{
		PX_ASSERT();
		return PX_NULL;
	}

	if (!PX_TextureCreate(mp, &pdesc->view_surface, width-24, height-24))
	{
		PX_ASSERT();
		return PX_NULL;
	}


	if (pdesc->cache_editing_surface.width*1.25f>pdesc->view_surface.width)
	{
		pdesc->min_scale = pdesc->view_surface.width * 1.0f / (pdesc->cache_editing_surface.width*1.25f);

		if (pdesc->min_scale > pdesc->view_surface.height * 1.0f / (pdesc->cache_editing_surface.height*1.25f))
		{
			pdesc->min_scale = pdesc->view_surface.height * 1.0f / (pdesc->cache_editing_surface.height*1.25f);
		}
	}
	pdesc->scale = pdesc->min_scale;

	pdesc->view_x = pdesc->cache_editing_surface.width / 2.f;
	pdesc->view_y = pdesc->cache_editing_surface.height / 2.f;
	PX_Object_CanvasRenderBackFrontLayersToCache(pObject);
	PX_Object_CanvasRenderEditingLayersToCache(pObject);
	PX_Object_CanvasRenderCacheToView(pObject);
	PX_Object_CanvasRenderCombineCache(pObject);
	PX_Object_CanvasRenderToLayerPreview(pObject);
	PX_Object_CanvasUpdateSliderBar(pObject);
	PX_ObjectRegisterEvent(pObject, PX_OBJECT_EVENT_CURSORWHEEL, PX_Object_CanvasOnCursorWheeling, 0);

	PX_ObjectRegisterEvent(pObject, PX_OBJECT_EVENT_CURSORDOWN, PX_Object_CanvasOnCursorDown, 0);
	PX_ObjectRegisterEvent(pObject, PX_OBJECT_EVENT_CURSORMOVE, PX_Object_CanvasOnCursorMove, 0);
	PX_ObjectRegisterEvent(pObject, PX_OBJECT_EVENT_CURSORDRAG, PX_Object_CanvasOnCursorDrag, 0);
	PX_ObjectRegisterEvent(pObject, PX_OBJECT_EVENT_CURSORMUP, PX_Object_CanvasOnCursorUp, 0);
	
	PX_ObjectRegisterEvent(pdesc->pHSliderBar, PX_OBJECT_EVENT_VALUECHANGED, PX_Object_CanvasOnHSliderBarChanged, pObject);
	PX_ObjectRegisterEvent(pdesc->pVSliderBar, PX_OBJECT_EVENT_VALUECHANGED, PX_Object_CanvasOnVSliderBarChanged, pObject);
	return pObject;
}

PX_Object_Canvas* PX_Object_GetCanvas(PX_Object* pObject)
{
	if (pObject->Type==PX_OBJECT_TYPE_CANVAS)
	{
		return PX_ObjectGetDesc(PX_Object_Canvas, pObject);
	}
	return PX_NULL;
}


