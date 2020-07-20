#include "rotoZoom.h"
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/memcheck.h>
#include <math.h>
#include <utilitieslib/assert/assert.h>

unsigned char charmax_tempa, charmax_tempb;
#define CHARMAX(a,b) ((charmax_tempa=((unsigned char)a))>(charmax_tempb=((unsigned char)b))?charmax_tempa:charmax_tempb)

typedef struct TexMapTable
{
    int x;
    int px;
    int py;
} TexMapTable;

static TexMapTable* m_leftTable;
static TexMapTable* m_rightTable;
static int m_tableSize;

// RotoZoom function (fast software blitting/transforming)
// ripped from CDXSurface from CDX (open source 2D graphicx library)
// hacked to support only 32-bit, additionally coloring, alpha values, x/yscaling

void Scanrightside(int x1,int x2,int ytop,int lineheight,char side,int TexWidth,int TexHeight,RECT* dClip)
{
    int x,px,py,xadd,pxadd,pyadd,y;

    if(lineheight < 1)lineheight=1;

    xadd = ((x2-x1)<<16)/lineheight;

    if(side==1)
    {
        px    = 0;
        py    = 0;
        pxadd = ((TexWidth) << 16) / (lineheight) - 1;
        pyadd = 0;
    }
    if(side==2)
    {
        px    = ((TexWidth-0) << 16) - 1;
        py    = 0;
        pxadd = 0;
//        if (0) {
            // Better filtering for non-rotated textures
            if (lineheight == 1)
                pyadd = (TexHeight << 16) / lineheight - 1;
            else {
                pyadd = ((TexHeight-1) << 16) / (lineheight-1);
                if (pyadd*(lineheight-1) >> 16 < TexHeight-1)    // Would miss the last line of the texture
                    pyadd++;
            }
//        } else {
//            pyadd = (TexHeight << 16) / lineheight - 1;
//        }
    }
    if(side==3)
    {
        px    = ((TexWidth - 0) << 16) - 1;
        py    = ((TexHeight - 0) << 16) - 1;
        pxadd = (-(TexWidth) << 16) / (lineheight) + 1;
        pyadd = 0;
    }
    if(side==4)
    {
        px    = 0;
        py    = ((TexHeight - 0) << 16) - 1;
        pxadd = 0;
//        if (0) {
//            if (lineheight == 1)
                pyadd = (-TexHeight << 16) / lineheight;
//            else
//                pyadd = (-(TexHeight-1) << 16) / (lineheight-1);
//            if (pyadd*(lineheight-1) >> 16 < TexHeight-1)    // Would miss the last line of the texture
//                pyadd++;
//        } else {
//            pyadd = (-TexHeight << 16) / lineheight + 1;
//        }
    }

    x = x1 << 16;

    for(y=0; y<lineheight; y++)
    {
        int yp=ytop+y;
        if( (yp >= dClip->top) && (yp < dClip->bottom) )
        {
            m_rightTable[yp].x  = x;
            m_rightTable[yp].px = px;
            m_rightTable[yp].py = py;
        }
        x  = x + xadd;
        px = px + pxadd;
        py = py + pyadd;
    }
}

void Scanleftside(int x1,int x2,int ytop,int lineheight,char side,int TexWidth,int TexHeight,RECT* dClip)
{
    int x,px,py,xadd,pxadd,pyadd,y;

    if(lineheight<1) lineheight = 1;

    xadd = ((x2-x1) << 16) / lineheight;

    if(side == 1)
    {
        px    = ((TexWidth - 0) << 16) - 1;
        py    = 0;
        pxadd = (-(TexWidth) << 16) / (lineheight) + 1;
        pyadd = 0;
    }
    if(side == 2)
    {
        px    = ((TexWidth - 0) << 16) - 1;
        py    = ((TexHeight - 0) << 16) - 1;
        pxadd = 0;
//        if (0) {
//            if (lineheight == 1)
                pyadd = (-TexHeight << 16) / lineheight + 1;
//            else
//                pyadd = (-(TexHeight-1) << 16) / (lineheight-1);
//            if (pyadd*(lineheight-1) >> 16 < TexHeight-1)    // Would miss the last line of the texture
//                pyadd++;
//        } else {
//            pyadd = (-TexHeight << 16) / lineheight - 1;
//        }
    }
    if(side == 3)
    {
        px    = 0;
        py    = ((TexHeight - 0) << 16) - 1;
        pxadd = ((TexWidth) << 16) / (lineheight) - 1;
        pyadd = 0;
    }
    if(side == 4)
    {
        px    = 0;
        py    = 0;
        pxadd = 0;
//        if (0) {
            if (lineheight == 1)
                pyadd = (TexHeight << 16) / lineheight - 1;
            else {
                pyadd = ((TexHeight-1) << 16) / (lineheight-1);
                if (pyadd*(lineheight-1) >> 16 < TexHeight-1)    // Would miss the last line of the texture
                    pyadd++;
            }
//        } else {
//            pyadd = (TexHeight << 16) / lineheight - 1;
//        }
    }

    x = x1 << 16;

    for(y=0; y<lineheight; y++)
    {
        int yp = ytop + y;

        if( (yp >= dClip->top) &&( yp < dClip->bottom) )
        {
            m_leftTable[yp].x  = x;
            m_leftTable[yp].px = px;
            m_leftTable[yp].py = py;
        }
        x  = x + xadd;
        px = px + pxadd;
        py = py + pyadd;
    }
}

void TextureMap(CDXSurface *src, CDXSurface *dest, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, RECT* area, Color rgba[], int numColors, int blend)
{
    BYTE* lpSrc;
    BYTE* lpDest;
    unsigned long dPitch, sPitch;
    RECT* dClipRect = &dest->clipRect;

    int TexWidth  = area->right  - area->left;
    int TexHeight = area->bottom - area->top;

    int polyx1,polyx2,y,linewidth,pxadd,pyadd;
    int texX,texY;

    int miny = y1;
    int maxy = y1;

    Color c, left, right;
    F32 horizw, vertw;

    unsigned int ofsmax = src->pitch*src->clipRect.bottom;

    if (numColors==1)
        c.integer = rgba[0].integer;

    //Screen->TexMapTableHeight = m_ClipRect.bottom;
    if (dClipRect->bottom > m_tableSize) {
        SAFE_FREE(m_leftTable);
        SAFE_FREE(m_rightTable);
        m_leftTable  = (TexMapTable*)calloc(dClipRect->bottom, sizeof(TexMapTable));
        m_rightTable = (TexMapTable*)calloc(dClipRect->bottom, sizeof(TexMapTable));
        m_tableSize = dClipRect->bottom;
    } else {
        //debug: memset(m_leftTable, 0xee, sizeof(TexMapTable)*m_tableSize);
        //debug: memset(m_rightTable, 0xee, sizeof(TexMapTable)*m_tableSize);
    }

    if(y2<miny) miny = y2;
    if(y2>maxy) maxy = y2;
    if(y3<miny) miny = y3;
    if(y3>maxy) maxy = y3;
    if(y4<miny) miny = y4;
    if(y4>maxy) maxy = y4;

    if(miny >= maxy) return;
    if(maxy < dClipRect->top) return;
    if(miny >= dClipRect->bottom)return;
    if(miny < dClipRect->top) miny = dClipRect->top;
    if(maxy > dClipRect->bottom) maxy = dClipRect->bottom;
    if(maxy - miny<1)return;

    if(y2 < y1) {Scanleftside(x2,x1,y2,y1-y2,1,TexWidth,TexHeight, dClipRect);}
    else        {Scanrightside(x1,x2,y1,y2-y1,1,TexWidth,TexHeight, dClipRect);}

    if(y3 < y2) {Scanleftside (x3,x2,y3,y2-y3,2,TexWidth,TexHeight, dClipRect);}
    else        {Scanrightside (x2,x3,y2,y3-y2,2,TexWidth,TexHeight, dClipRect);}

    if(y4 < y3) {Scanleftside (x4,x3,y4,y3-y4,3,TexWidth,TexHeight, dClipRect);}
    else        {Scanrightside (x3,x4,y3,y4-y3,3,TexWidth,TexHeight, dClipRect);}

    if(y1 < y4) {Scanleftside (x1,x4,y1,y4-y1,4,TexWidth,TexHeight, dClipRect);}
    else        {Scanrightside (x4,x1,y4,y1-y4,4,TexWidth,TexHeight, dClipRect);}

    // Set the pitch for both surfaces
    sPitch = src->pitch;
    dPitch = dest->pitch;

    // Initialize the pointers to the upper left hand corner of the surface
    lpSrc  = (BYTE*)src->buffer;
    lpDest = (BYTE*)dest->buffer;

//    if(m_PixelFormat.bpp == 32)
    {

        for(y=miny; y<maxy; y++)
        {
            int x;
            unsigned char* dst;
            polyx1 = m_leftTable[y].x >> 16;
            polyx2 = m_rightTable[y].x >> 16;
            linewidth = polyx2 - polyx1;
            if(linewidth < 1) linewidth = 1;
            pxadd=((m_rightTable[y].px) - (m_leftTable[y].px)) / linewidth;
            pyadd=((m_rightTable[y].py) - (m_leftTable[y].py)) / linewidth;

            texX = m_leftTable[y].px;
            texY = m_leftTable[y].py;

            if(polyx1 < dClipRect->left)
            {
                texX  += pxadd * (dClipRect->left - polyx1);
                texY  += pyadd * (dClipRect->left - polyx1);
                polyx1 = dClipRect->left;
            }
            if(polyx2 > dClipRect->right - 0) {polyx2 = dClipRect->right - 0;}

            texX += area->left << 16;
            texY += area->top << 16;
            dst = (lpDest + (y * dPitch) + (polyx1 * 4) );

            if (numColors==4) {
                // colorize!
                for(x=polyx1; x<polyx2; x++)
                {
                    unsigned int ofs;

#define UL 2
#define UR 3
#define LR 0
#define LL 1
                    horizw = (texX >> 16) / (F32)TexWidth;
                    vertw = (texY >> 16) / (F32)TexHeight;
                    // c = lerp(lerp(cUL, cLL, yp), lerp(cUR, cLR, yp), xp)
                    left.r = rgba[UL].r*vertw + rgba[LL].r*(1-vertw);
                    left.g = rgba[UL].g*vertw + rgba[LL].g*(1-vertw);
                    left.b = rgba[UL].b*vertw + rgba[LL].b*(1-vertw);
                    left.a = rgba[UL].a*vertw + rgba[LL].a*(1-vertw);
                    right.r = rgba[UR].r*vertw + rgba[LR].r*(1-vertw);
                    right.g = rgba[UR].g*vertw + rgba[LR].g*(1-vertw);
                    right.b = rgba[UR].b*vertw + rgba[LR].b*(1-vertw);
                    right.a = rgba[UR].a*vertw + rgba[LR].a*(1-vertw);
                    c.r = left.r * horizw + right.r * (1-horizw);
                    c.g = left.g * horizw + right.g * (1-horizw);
                    c.b = left.b * horizw + right.b * (1-horizw);
                    c.a = left.a * horizw + right.a * (1-horizw);

                    ofs = (texY >> 16) * sPitch + (texX >> 16) * 4;
                    assert(ofs < ofsmax);
                    if (blend) {
                        char srcalpha = *(lpSrc + ofs + 3) * c.a >> 8;
                        if (srcalpha) {
                            char dstalpha = *(dst + 3);
                            if (!dstalpha) {
                                *dst++ = *(lpSrc + ofs) * c.r >> 8;
                                *dst++ = *(lpSrc + ofs + 1) * c.g >> 8;
                                *dst++ = *(lpSrc + ofs + 2) * c.b >> 8;
                                *dst++ = srcalpha;
                            } else {
                                // do a blend
                                *dst++ = CHARMAX(*(lpSrc + ofs) * c.r >> 8, *dst);
                                *dst++ = CHARMAX(*(lpSrc + ofs + 1) * c.g >> 8, *dst);
                                *dst++ = CHARMAX(*(lpSrc + ofs + 2) * c.b >> 8, *dst);
                                *dst++ = CHARMAX(srcalpha, dstalpha);
                            }
                        } else {
                            dst+=4;
                        }
                    } else {
                        *dst++ = *(lpSrc + ofs) * c.r >> 8;
                        *dst++ = *(lpSrc + ofs + 1) * c.g >> 8;
                        *dst++ = *(lpSrc + ofs + 2) * c.b >> 8;
                        *dst++ = *(lpSrc + ofs + 3) * c.a >> 8;
                    }

                    texX += pxadd;
                    texY += pyadd;
                }
            } else if (numColors==1) {
                for(x=polyx1; x<polyx2; x++)
                {
                    unsigned int ofs;

                    ofs = (texY >> 16) * sPitch + (texX >> 16) * 4;
                    assert(ofs < ofsmax);

                    if (blend) {
                        char srcalpha = *(lpSrc + ofs + 3) * c.a >> 8;
                        if (srcalpha) {
                            char dstalpha = *(dst + 3);
                            if (!dstalpha) {
                                *dst++ = *(lpSrc + ofs) * c.r >> 8;
                                *dst++ = *(lpSrc + ofs + 1) * c.g >> 8;
                                *dst++ = *(lpSrc + ofs + 2) * c.b >> 8;
                                *dst++ = srcalpha;
                            } else {
                                // do a blend
                                *dst++ = CHARMAX(*(lpSrc + ofs) * c.r >> 8, *dst);
                                *dst++ = CHARMAX(*(lpSrc + ofs + 1) * c.g >> 8, *dst);
                                *dst++ = CHARMAX(*(lpSrc + ofs + 2) * c.b >> 8, *dst);
                                *dst++ = CHARMAX(srcalpha, dstalpha);
                            }
                        } else {
                            dst+=4;
                        }
                    } else {
                        *dst++ = *(lpSrc + ofs) * c.r >> 8;
                        *dst++ = *(lpSrc + ofs + 1) * c.g >> 8;
                        *dst++ = *(lpSrc + ofs + 2) * c.b >> 8;
                        *dst++ = *(lpSrc + ofs + 3) * c.a >> 8;
                    }

                    texX += pxadd;
                    texY += pyadd;
                }
            } else {
                for(x=polyx1; x<polyx2; x++)
                {
                    unsigned int ofs;

                    ofs = (texY >> 16) * sPitch + (texX >> 16) * 4;
                    assert(ofs < ofsmax);
                    if (blend) {
                        char srcalpha = *(lpSrc + ofs + 3);
                        if (srcalpha) {
                            char dstalpha = *(dst + 3);
                            if (!dstalpha) {
                                *dst++ = *(lpSrc + ofs);
                                *dst++ = *(lpSrc + ofs + 1);
                                *dst++ = *(lpSrc + ofs + 2);
                                *dst++ = srcalpha;
                            } else {
                                // do a blend
                                *dst++ = CHARMAX(*(lpSrc + ofs), *dst);
                                *dst++ = CHARMAX(*(lpSrc + ofs + 1), *dst);
                                *dst++ = CHARMAX(*(lpSrc + ofs + 2), *dst);
                                *dst++ = CHARMAX(srcalpha, dstalpha);
                            }
                        } else {
                            dst+=4;
                        }
                    } else {
                        *dst++ = *(lpSrc + ofs);
                        *dst++ = *(lpSrc + ofs + 1);
                        *dst++ = *(lpSrc + ofs + 2);
                        *dst++ = *(lpSrc + ofs + 3);
                    }

                    texX += pxadd;
                    texY += pyadd;
                }
            }
        }
    }
}

int DrawBlkRotoZoom(CDXSurface *src, CDXSurface* dest, int midX, int midY, RECT* srcArea, double angle, double scalex, double scaley, Color rgba[], int numColors, int blend)
{
    double SinA,CosA;

    // Clip the source rect to the extents of the surface.
    if (srcArea->top >= src->clipRect.bottom)
        return FALSE;

    if (srcArea->left >= src->clipRect.right)
        return FALSE;

    if (srcArea->bottom <= src->clipRect.top)
        return FALSE;

    if (srcArea->right <= src->clipRect.left)
        return FALSE;

    // Clip rect to the surface's clipRect
    if (srcArea->top < src->clipRect.top)
        srcArea->top = src->clipRect.top;

    if (srcArea->left < src->clipRect.left)
        srcArea->left = src->clipRect.left;

    if (srcArea->bottom > src->clipRect.bottom)
        srcArea->bottom = src->clipRect.bottom;

    if (srcArea->right > src->clipRect.right)
        srcArea->right = src->clipRect.right;

    {
        int HalfWidth  = (int)((srcArea->right  - srcArea->left) * scalex / 2);
        int HalfHeight = (int)((srcArea->bottom - srcArea->top)  * scaley / 2);
        int xRemainder = (int)((srcArea->right  - srcArea->left) * scalex) % 2;
        int yRemainder = (int)((srcArea->bottom - srcArea->top) * scaley) % 2;
        int negHalfWidth = -HalfWidth;
        int negHalfHeight = -HalfHeight;
        HalfWidth+=xRemainder;
        HalfHeight+=yRemainder;

        SinA = sin(-angle);
        CosA = cos(-angle);

        {
            int x1 = (int)(CosA * negHalfWidth - SinA * negHalfHeight);
            int y1 = (int)(SinA * negHalfWidth + CosA * negHalfHeight);
            int x2 = (int)(CosA * HalfWidth - SinA * negHalfHeight);
            int y2 = (int)(SinA * HalfWidth + CosA * negHalfHeight);
            int x3 = (int)(CosA * HalfWidth - SinA * HalfHeight);
            int y3 = (int)(SinA * HalfWidth + CosA * HalfHeight);
            int x4 = (int)(CosA * negHalfWidth - SinA * HalfHeight);
            int y4 = (int)(SinA * negHalfWidth + CosA * HalfHeight);

            TextureMap(src, dest, x1+midX, y1+midY, x2+midX, y2+midY, x3+midX, y3+midY, x4+midX, y4+midY, srcArea, rgba, numColors, blend);
        }
    }
    return 1;
}
