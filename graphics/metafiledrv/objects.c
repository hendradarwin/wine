/*
 * GDI objects
 *
 * Copyright 1993 Alexandre Julliard
 */

#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"
#include "brush.h"
#include "font.h"
#include "metafile.h"
#include "metafiledrv.h"
#include "pen.h"
#include "stddebug.h"
#include "debug.h"


/***********************************************************************
 *           MFDRV_BITMAP_SelectObject
 */
static HBITMAP16 MFDRV_BITMAP_SelectObject( DC * dc, HBITMAP16 hbitmap,
                                            BITMAPOBJ * bmp )
{
    return 0;
}


/***********************************************************************
 *           MFDRV_BRUSH_SelectObject
 */
static HBRUSH32 MFDRV_BRUSH_SelectObject( DC * dc, HBRUSH32 hbrush,
                                          BRUSHOBJ * brush )
{
    LOGBRUSH16 logbrush = { brush->logbrush.lbStyle,
                            brush->logbrush.lbColor,
                            brush->logbrush.lbHatch };
    switch (brush->logbrush.lbStyle)
    {
    case BS_SOLID:
    case BS_HATCHED:
    case BS_HOLLOW:
        if (!MF_CreateBrushIndirect( dc, hbrush, &logbrush )) return 0;
        break;
    case BS_PATTERN:
    case BS_DIBPATTERN:
        if (!MF_CreatePatternBrush( dc, hbrush, &logbrush )) return 0;
        break;
    }
    return 1;  /* FIXME? */
}


/***********************************************************************
 *           MFDRV_FONT_SelectObject
 */
static HFONT16 MFDRV_FONT_SelectObject( DC * dc, HFONT16 hfont,
                                        FONTOBJ * font )
{
    HFONT16 prevHandle = dc->w.hFont;
    if (MF_CreateFontIndirect(dc, hfont, &(font->logfont))) return prevHandle;
    return 0;
}


/***********************************************************************
 *           MFDRV_PEN_SelectObject
 */
static HPEN32 MFDRV_PEN_SelectObject( DC * dc, HPEN32 hpen, PENOBJ * pen )
{
    HPEN32 prevHandle = dc->w.hPen;
    LOGPEN16 logpen = { pen->logpen.lopnStyle,
                        { pen->logpen.lopnWidth.x, pen->logpen.lopnWidth.y },
                        pen->logpen.lopnColor };
    if (MF_CreatePenIndirect( dc, hpen, &logpen )) return prevHandle;
    return 0;
}


/***********************************************************************
 *           MFDRV_SelectObject
 */
HGDIOBJ32 MFDRV_SelectObject( DC *dc, HGDIOBJ32 handle )
{
    GDIOBJHDR * ptr = GDI_GetObjPtr( handle, MAGIC_DONTCARE );

    if (!ptr) return 0;
    dprintf_gdi(stddeb, "SelectObject: hdc=%04x %04x\n", dc->hSelf, handle );
    
    switch(ptr->wMagic)
    {
      case PEN_MAGIC:
	  return MFDRV_PEN_SelectObject( dc, handle, (PENOBJ *)ptr );
      case BRUSH_MAGIC:
	  return MFDRV_BRUSH_SelectObject( dc, handle, (BRUSHOBJ *)ptr );
      case BITMAP_MAGIC:
	  return MFDRV_BITMAP_SelectObject( dc, handle, (BITMAPOBJ *)ptr );
      case FONT_MAGIC:
	  return MFDRV_FONT_SelectObject( dc, handle, (FONTOBJ *)ptr );	  
      case REGION_MAGIC:
	  return (HGDIOBJ16)SelectClipRgn16( dc->hSelf, handle );
    }
    return 0;
}
