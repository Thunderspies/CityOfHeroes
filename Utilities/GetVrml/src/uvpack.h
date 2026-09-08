#ifndef _UVPACK_H_
#define _UVPACK_H_

#include <utilitieslib/utils/tga.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>

// this code is a mess, but all of these things must be defined before prim.h and combinedpoly.h
/**/#include "uvrect.h"

/**/#define BORDER_SIZE 1
/**/#define DOUBLE_BORDER_SIZE 2
/**/#define MAXCPSIZE 512
/**/#define MAX_SCALE 3

/**/#define FILL_HOLES 0
/**/#define POWEROFTWO_OUTPUT 0
/**/#define COMBINE_POLYS 1

/**/#define REMAPDEBUG 0
/**/#define DEBUG_PRIM 198
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "prim.h"
#include "combinedpoly.h"

#ifdef EPSILON 
#undef EPSILON
#endif
#define EPSILON (1.0e-5)

#define TGASIZE 512
#define NEW_BORDER_SIZE 2.0f

// needed this to make earrays of Vec2
typedef struct {
    Vec2 vec;
} Vec2D;

#define Vec2Equals(a,b) (fabs((a)[0]-(b)[0]) < EPSILON && fabs((a)[1]-(b)[1]) < EPSILON)
#define Vec2DEquals(a,b) (fabs((a)->vec[0]-(b)->vec[0]) < EPSILON && fabs((a)->vec[1]-(b)->vec[1]) < EPSILON)
#define Vec2DEqualsVec2(v2d,v2) (fabs((v2d)->vec[0]-(v2)[0]) < EPSILON && fabs((v2d)->vec[1]-(v2)[1]) < EPSILON)

/**************************************************************

Debug Code

**************************************************************/

static char *logFilePath = "C:\\lmlog.txt";
static FILE *logFile = NULL;
// just to make memory management easier, all spare verts that need to be allocated will be in this earray
static Vec2D **spareVerts;

Vec2D *createSpareVert()
{
    Vec2D *ret = (Vec2D*)malloc(sizeof(Vec2D));
    eaPush(&spareVerts, ret);
    return ret;
}


void simpleLog( char *text )
{
    if ( !logFile )
        logFile = fopen( logFilePath, "wt" );

    fprintf( logFile, "%s", text );
}

void simpleLogClear()
{
    if ( logFile )
    {
        fclose( logFile );
        logFile = fopen( logFilePath, "wt" );
    }
}

void simpleLogf( char *fmt, ... )
{
    va_list argptr;
    char buf[4096];

    va_start(argptr, fmt);
    vsprintf( buf, fmt, argptr );
    simpleLog( buf );
    va_end(argptr);
}

void printPrimitive( Prim *p, FILE *f, char *prefix )
{
#define UV_STRING(PRINT_PRECISION) "u1:%" #PRINT_PRECISION "f v1:%" #PRINT_PRECISION "f\tu2:%" #PRINT_PRECISION "f v2:%" #PRINT_PRECISION "f\tu3:%" #PRINT_PRECISION "f v3:%" #PRINT_PRECISION "f"
    if ( prefix )
    {
        if ( f )
            fprintf(f, "%s" UV_STRING(0.3), 
            prefix, p->uv[0][0], p->uv[0][1], p->uv[1][0], p->uv[1][1], p->uv[2][0], p->uv[2][1]);
        else 
            printf( "%s" UV_STRING(0.3), 
            prefix, p->uv[0][0], p->uv[0][1], p->uv[1][0], p->uv[1][1], p->uv[2][0], p->uv[2][1]);
    }
    else
    {
        if ( f )
            fprintf(f, UV_STRING(0.3),    
            prefix, p->uv[0][0], p->uv[0][1], p->uv[1][0], p->uv[1][1], p->uv[2][0], p->uv[2][1]);
        else 
            printf( UV_STRING(0.3),    
            prefix, p->uv[0][0], p->uv[0][1], p->uv[1][0], p->uv[1][1], p->uv[2][0], p->uv[2][1]);
    }
}

void printAllPrimitivesFile( Prim ***p, char *path )
{
    FILE *file = fopen(path, "wt");
    int i;

    for ( i = 0; i < eaSize(p); ++i )
    {
        fprintf( file, "Primitive %d", i );
        printPrimitive( (*p)[i], file, "\t" );
        fprintf( file, "\n" );
    }

    fclose(file);
}

void printAllPrimitives( Prim ***p )
{
    int i;
    for ( i = 0; i < eaSize(p); ++i )
    {
        printf( "Primitive %d", i );
        printPrimitive( (*p)[i], NULL, "\t" );
        printf( "\n" );
    }
}

void printAllVec2( Vec2 **v )
{
    int i;
    for ( i = 0; i < eaSize((void***)(&v)); ++i )
    {
        printf( "%f, %f\n", v[i][0], v[i][1] );
    }
}

void printAllVec2D( Vec2D **v )
{
    int i;
    for ( i = 0; i < eaSize(&v); ++i )
    {
        printf( "%f, %f\n", v[i]->vec[0], v[i]->vec[1] );
    }
}

void printAllCPs( CombinedPoly ***cp, char *path )
{
    int i;
    char newPath[256];

    for ( i = 0; i < eaSize(cp); ++i )
    {
        sprintf( newPath, "%s\\%d.txt", path, i );
        printAllPrimitives( &((*cp)[i]->primitives) );
    }
}

void storeAllPrimitives( Prim ***p, char *path )
{
    FILE * file = fopen(path, "wb");
    int i;

    for ( i = 0; i < eaSize(p); ++i )
    {
        fwrite( (*p)[i]->uv[0], sizeof(F32), 2, file );
        fwrite( (*p)[i]->uv[1], sizeof(F32), 2, file );
        fwrite( (*p)[i]->uv[2], sizeof(F32), 2, file );
    }
}

void printHoriz( Vec2D ***horiz )
{
    int i = 0;
    for (; i < eaSize(horiz); ++i)
    {
        printf("\t%d: %f, %f\n", i, (*horiz)[i]->vec[0], (*horiz)[i]->vec[1]);
    }
}

void printHorizAddr( Vec2D ***horiz )
{
    int i = 0;
    for (; i < eaSize(horiz); ++i)
    {
        printf("\t%d: %x\n", i, (*horiz)[i] );
    }
}

void assertPointersInHoriz( Vec2D ***horiz )
{
    int i = 0;
    for (; i < eaSize(horiz); ++i)
    {
        float f;
        assert( (*horiz)[i] );
        assert( (*horiz)[i]->vec );
        f = (*horiz)[i]->vec[0];
        f = (*horiz)[i]->vec[1];
    }

}


int tga_pix[TGASIZE][TGASIZE] = {0,};
static int red = 0xFF0000FF, green = 0xFF00FF00, blue = 0xFFFF0000;

//#define setPixel(x, y, color) (tga_pix[(x)][(y)] = (color))
#define checkPixel(x, y){assert( (x) < TGASIZE && (x) >= 0 ); assert( (y) < TGASIZE && (y) >= 0 );}
void setPixel( int x, int y, int color )
{
    checkPixel(x,y);
    tga_pix[y][x] = color;
}

void drawTGAStart()
{
    int i, j;
    for ( i = 0; i < TGASIZE; ++i )
    {
        for ( j = 0; j < TGASIZE; ++j )
        {
            tga_pix[i][j] = 0;
        }
    }
}

void drawTGAEnd(char *path)
{
    tgaSave( path, (char *)tga_pix, TGASIZE, TGASIZE, 4 );
}


void drawLineTGA(int x0, int y0, int x1, int y1, int color)
{
    int dy = y1 - y0;
    int dx = x1 - x0;
    float t = (float) 0.5;                      // offset for rounding

    setPixel(x0, y0, color);
    if (abs(dx) > abs(dy)) {                    // slope < 1
        float m = (float) dy / (float) dx;      // compute slope
        t += y0;
        dx = (dx < 0) ? -1 : 1;
        m *= dx;
        while (x0 != x1) {
            x0 += dx;                           // step to next x value
            t += m;                             // add slope to y value
            setPixel(x0, (int) t, color);
        }
    } else {                                    // slope >= 1
        float m = (float) dx / (float) dy;      // compute slope
        t += x0;
        dy = (dy < 0) ? -1 : 1;
        m *= dy;
        while (y0 != y1) {
            y0 += dy;                           // step to next y value
            t += m;                             // add slope to x value
            setPixel((int) t, y0, color);
        }
    }
}


void drawHorizonTGA( Vec2D *** horiz, int width, char *path, int iter )
{
    float scale = TGASIZE / (float)width;
    int i;
    char newPath[256];

    drawTGAStart();

    if ( iter < 10 )
        sprintf( newPath, "%s\\0%d.tga", path, iter );
    else
        sprintf( newPath, "%s\\%d.tga", path, iter );

    for ( i = 0; i < eaSize(horiz)-1; ++i )
    {
        drawLineTGA( (*horiz)[i]->vec[0] * scale, (*horiz)[i]->vec[1] * scale,
            (*horiz)[i+1]->vec[0] * scale, (*horiz)[i+1]->vec[1] * scale, red );
    }

    drawTGAEnd( newPath );
}

void drawHorizonTGANoFinal( Vec2D *** horiz, int width, int color )
{
    float scale = TGASIZE / (float)width;
    int i, x1, y1, x2, y2;

    for ( i = 0; i < eaSize(horiz)-1; ++i )
    {
        x1 = (*horiz)[i]->vec[0] * scale;
        y1 = (*horiz)[i]->vec[1] * scale;
        x2 = (*horiz)[i+1]->vec[0] * scale;
        y2 = (*horiz)[i+1]->vec[1] * scale;
        if ( x1 >= TGASIZE )
            x1 = TGASIZE - 1;
        if ( y1 >= TGASIZE )
            y1 = TGASIZE - 1;
        if ( x2 >= TGASIZE )
            x2 = TGASIZE - 1;
        if ( y2 >= TGASIZE )
            y2 = TGASIZE - 1;
        checkPixel(x1, y1);
        checkPixel(x2, y2);
        drawLineTGA( x1, y1, x2, y2, color );
    }
}

void drawHorizonOffsetTGANoFinal( Vec2D *** horiz, int x_off, int y_off, int width, int color )
{
    float scale = TGASIZE / (float)width;
    int i, x1, y1, x2, y2;

    for ( i = 0; i < eaSize(horiz)-1; ++i )
    {
        x1 = ((*horiz)[i]->vec[0] + x_off) * scale;
        y1 = ((*horiz)[i]->vec[1] + y_off) * scale;
        x2 = ((*horiz)[i+1]->vec[0] + x_off) * scale;
        y2 = ((*horiz)[i+1]->vec[1] + y_off) * scale;
        if ( x1 >= TGASIZE )
            x1 = TGASIZE - 1;
        if ( y1 >= TGASIZE )
            y1 = TGASIZE - 1;
        if ( x2 >= TGASIZE )
            x2 = TGASIZE - 1;
        if ( y2 >= TGASIZE )
            y2 = TGASIZE - 1;
        checkPixel(x1, y1);
        checkPixel(x2, y2);
        drawLineTGA( x1, y1, x2, y2, color );
    }
}


void drawHorizonsTGA( Vec2D ***top, Vec2D ***bottom, Vec2D ***cur, int width, char *path, int iter )
{
    float scale = TGASIZE / (float)width;
    int i, x1, y1, x2, y2;
    char newPath[256];

    drawTGAStart();

    if ( iter < 10 )
        sprintf( newPath, "%s\\0%d.tga", path, iter );
    else
        sprintf( newPath, "%s\\%d.tga", path, iter );

    if ( top )
    {
        for ( i = 0; i < eaSize(top)-1; ++i )
        {
            x1 = (*top)[i]->vec[0] * scale;
            y1 = (*top)[i]->vec[1] * scale;
            x2 = (*top)[i+1]->vec[0] * scale;
            y2 = (*top)[i+1]->vec[1] * scale;
            if ( x1 >= TGASIZE )
                x1 = TGASIZE - 1;
            if ( y1 >= TGASIZE )
                y1 = TGASIZE - 1;
            if ( x2 >= TGASIZE )
                x2 = TGASIZE - 1;
            if ( y2 >= TGASIZE )
                y2 = TGASIZE - 1;
            checkPixel(x1, y1);
            checkPixel(x2, y2);
            drawLineTGA( x1, y1, x2, y2, blue );
        }
    }
    if ( bottom )
    {
        for ( i = 0; i < eaSize(bottom)-1; ++i )
        {
            x1 = (*bottom)[i]->vec[0] * scale;
            y1 = (*bottom)[i]->vec[1] * scale;
            x2 = (*bottom)[i+1]->vec[0] * scale;
            y2 = (*bottom)[i+1]->vec[1] * scale;
            if ( x1 >= TGASIZE )
                x1 = TGASIZE - 1;
            if ( y1 >= TGASIZE )
                y1 = TGASIZE - 1;
            if ( x2 >= TGASIZE )
                x2 = TGASIZE - 1;
            if ( y2 >= TGASIZE )
                y2 = TGASIZE - 1;
            checkPixel(x1, y1);
            checkPixel(x2, y2);
            drawLineTGA( x1, y1, x2, y2, green );
        }
    }
    if ( cur )
    {
        for ( i = 0; i < eaSize(cur)-1; ++i )
        {
            x1 = (*cur)[i]->vec[0] * scale;
            y1 = (*cur)[i]->vec[1] * scale;
            x2 = (*cur)[i+1]->vec[0] * scale;
            y2 = (*cur)[i+1]->vec[1] * scale;
            if ( x1 >= TGASIZE )
                x1 = TGASIZE - 1;
            if ( y1 >= TGASIZE )
                y1 = TGASIZE - 1;
            if ( x2 >= TGASIZE )
                x2 = TGASIZE - 1;
            if ( y2 >= TGASIZE )
                y2 = TGASIZE - 1;
            checkPixel(x1, y1);
            checkPixel(x2, y2);
            drawLineTGA( x1, y1, x2, y2, red );
        }
    }
    drawTGAEnd( newPath );
}

void drawCPTGA( CombinedPoly *cp, int width, char *path, int iter )
{
    float scale = TGASIZE / (float)width;
    int i, x1, y1, x2, y2, x3, y3;
    char newPath[256];

    drawTGAStart();

    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        Prim *p = cp->primitives[i];
        x1 = p->uv[0][0] * scale;
        y1 = p->uv[0][1] * scale;
        x2 = p->uv[1][0] * scale;
        y2 = p->uv[1][1] * scale;
        x3 = p->uv[2][0] * scale;
        y3 = p->uv[2][1] * scale;
        if ( x1 >= TGASIZE )
            x1 = TGASIZE - 1;
        if ( y1 >= TGASIZE )
            y1 = TGASIZE - 1;
        if ( x2 >= TGASIZE )
            x2 = TGASIZE - 1;
        if ( y2 >= TGASIZE )
            y2 = TGASIZE - 1;
        if ( x3 >= TGASIZE )
            x3 = TGASIZE - 1;
        if ( y3 >= TGASIZE )
            y3 = TGASIZE - 1;
        checkPixel(x1, y1);
        checkPixel(x2, y2);
        checkPixel(x3, y3);
        drawLineTGA( x1, y1, x2, y2, blue );
        drawLineTGA( x2, y2, x3, y3, blue );
        drawLineTGA( x3, y3, x1, y1, blue );
    }

    if ( iter < 10 )
        sprintf( newPath, "%s\\0%d.tga", path, iter );
    else
        sprintf( newPath, "%s\\%d.tga", path, iter );
    drawTGAEnd( newPath );
}

void drawCPTGANoFinal( CombinedPoly *cp, int width, int color )
{
    float scale = TGASIZE / (float)width;
    int i, x1, y1, x2, y2, x3, y3;

    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        Prim *p = cp->primitives[i];
        x1 = p->uv[0][0] * scale;
        y1 = p->uv[0][1] * scale;
        x2 = p->uv[1][0] * scale;
        y2 = p->uv[1][1] * scale;
        x3 = p->uv[2][0] * scale;
        y3 = p->uv[2][1] * scale;
        if ( x1 >= TGASIZE )
            x1 = TGASIZE - 1;
        if ( y1 >= TGASIZE )
            y1 = TGASIZE - 1;
        if ( x2 >= TGASIZE )
            x2 = TGASIZE - 1;
        if ( y2 >= TGASIZE )
            y2 = TGASIZE - 1;
        if ( x3 >= TGASIZE )
            x3 = TGASIZE - 1;
        if ( y3 >= TGASIZE )
            y3 = TGASIZE - 1;
        checkPixel(x1, y1);
        checkPixel(x2, y2);
        checkPixel(x3, y3);
        drawLineTGA( x1, y1, x2, y2, color );
        drawLineTGA( x2, y2, x3, y3, color );
        drawLineTGA( x3, y3, x1, y1, color );
    }
}

void drawPrimTGANoFinal(Prim *p, int width, int color )
{
    float scale = TGASIZE / (float)width;
    int x1, y1, x2, y2, x3, y3;

    x1 = p->uv[0][0] * scale;
    y1 = p->uv[0][1] * scale;
    x2 = p->uv[1][0] * scale;
    y2 = p->uv[1][1] * scale;
    x3 = p->uv[2][0] * scale;
    y3 = p->uv[2][1] * scale;
    if ( x1 >= TGASIZE )
        x1 = TGASIZE - 1;
    if ( y1 >= TGASIZE )
        y1 = TGASIZE - 1;
    if ( x2 >= TGASIZE )
        x2 = TGASIZE - 1;
    if ( y2 >= TGASIZE )
        y2 = TGASIZE - 1;
    if ( x3 >= TGASIZE )
        x3 = TGASIZE - 1;
    if ( y3 >= TGASIZE )
        y3 = TGASIZE - 1;
    checkPixel(x1, y1);
    checkPixel(x2, y2);
    checkPixel(x3, y3);
    drawLineTGA( x1, y1, x2, y2, color );
    drawLineTGA( x2, y2, x3, y3, color );
    drawLineTGA( x3, y3, x1, y1, color );
}

void printPerformanceNode(PerformanceInfo *n)
{
    PerformanceInfo *cur = n->child.head;
    static int depth = 0;
    int i;
    S64 avg_time = n->totalTime/n->opCountInt;

    i = depth;
    while ( i )
    {
        --i;
        simpleLog("\t");
    }

    simpleLogf("%s - time: %I64d (%f s)\tcalls: %I64d\tavg time:%I64d (%f s)\n", 
        n->locName, n->totalTime, n->totalTime/(F32)getRegistryMhz(), n->opCountInt, 
        avg_time, avg_time/(F32)getRegistryMhz());

    while ( cur )
    {
        ++depth;
        printPerformanceNode(cur);
        --depth;
        cur = cur->nextSibling;
    }

}

void printPerformanceInfo()
{
    PerformanceInfo *root = timing_state.autoTimerRootList, *cur;
    S64 totalTime = timing_state.totalTime;

//    simpleLogf("%s : %d", root->locName, root->totalTime);
    cur = root->nextSibling;

    while ( stricmp(cur->locName, "nothing") == 0 ) cur = cur->nextSibling;

    printPerformanceNode(cur);

    fclose(logFile);
}



/**************************************************************

End Debug Code

**************************************************************/




// from http://www.ggobi.org/docs/api/xlines_8c-source.html
int lines_intersect (
    F32 x1, F32 y1, F32 x2, F32 y2, /* First line segment */
    F32 x3, F32 y3, F32 x4, F32 y4) /* Second line segment */
    /*glong *x, glong *y)*/     /* Output value: * point of intersection */
{
    F32 a1, a2, b1, b2, c1, c2; /* Coefficients of line eqns. */
    F32 r1, r2, r3, r4;         /* 'Sign' values */
    F32 denom;                  /* Intermediate values */

#define SAME_SIGNS( a, b ) ( ((*(U32*)&(a)) & 0x80000000) == ((*(U32*)&(b)) & 0x80000000) )

    /* Compute a1, b1, c1, where line joining points 1 and 2
    * is "a1 x  +  b1 y  +  c1  =  0".
    */

//PERFINFO_AUTO_START("lines_intersect",1);

    a1 = y2 - y1;
    b1 = x1 - x2;
    c1 = x2 * y1 - x1 * y2;

    /*-- Compute r3 and r4.  --*/
    r3 = a1 * x3 + b1 * y3 + c1;
    r4 = a1 * x4 + b1 * y4 + c1;

    /* Check signs of r3 and r4.  If both point 3 and point 4 lie on
    * same side of line 1, the line segments do not intersect.
    */

    if (r3 != 0 && r4 != 0 && SAME_SIGNS( r3, r4 ))
    {
        //PERFINFO_AUTO_STOP();
        return 0;
    }

    /* Compute a2, b2, c2 */

    a2 = y4 - y3;
    b2 = x3 - x4;
    c2 = x4 * y3 - x3 * y4;

    /* Compute r1 and r2 */

    r1 = a2 * x1 + b2 * y1 + c2;
    r2 = a2 * x2 + b2 * y2 + c2;

    /* Check signs of r1 and r2.  If both point 1 and point 2 lie
    * on same side of second line segment, the line segments do
    * not intersect.
    */

    if ( r1 != 0 && r2 != 0 && SAME_SIGNS( r1, r2 ))
    {
        //PERFINFO_AUTO_STOP();    
        return 0;
    }

    /* Line segments intersect: compute intersection point. */

    denom = a1 * b2 - a2 * b1;
    if ( denom == 0 )
    {
        // collinear lines
        if ( x2 - x1 == 0 )
        {
            if ( y2 < y1 )
                SWAP32(y2, y1);
            if ( y4 < y3 )
                SWAP32(y4, y3);

            if ( (y3 <= y2 && y3 >= y1) || (y4 <= y2 && y4 >= y1) ||
                (y3 <= y2 && y3 >= y1) || (y4 <= y2 && y4 >= y1) )                
            {
                //PERFINFO_AUTO_STOP();
                return 1;
            }
        }
        else
        {
            if ( x2 < x1 )
                SWAP32(x2, x1);
            if ( x4 < x3 )
                SWAP32(x4, x3);

            if ( (x3 <= x2 && x3 >= x1) || (x4 <= x2 && x4 >= x1) ||
                (x3 <= x2 && x3 >= x1) || (x4 <= x2 && x4 >= x1) )
            {
                //PERFINFO_AUTO_STOP();
                return 1;
            }
        }
        //PERFINFO_AUTO_STOP();
        return 0;
    }

    /* The denom/2 is to get rounding instead of truncating.  It
    * is added or subtracted to the numerator, depending upon the
    * sign of the numerator.
    */

/*
glong num, offset;
    offset = denom < 0 ? - denom / 2 : denom / 2;
    num = b1 * c2 - b2 * c1;
    *x = ( num < 0 ? num - offset : num + offset ) / denom;
    num = a2 * c1 - a1 * c2;
    *y = ( num < 0 ? num - offset : num + offset ) / denom;
*/

    //PERFINFO_AUTO_STOP();
    return 1;
} /* lines_intersect */


/* exactly the same as above, except only returns true on collinear lines */
int lines_are_collinear (
    F32 x1, F32 y1, F32 x2, F32 y2, /* First line segment */
    F32 x3, F32 y3, F32 x4, F32 y4) /* Second line segment */
    /*glong *x, glong *y)*/     /* Output value: * point of intersection */
{
    F32 a1, a2, b1, b2, c1, c2; /* Coefficients of line eqns. */
    F32 r1, r2, r3, r4;         /* 'Sign' values */
    F32 denom;                  /* Intermediate values */

//#define SAME_SIGNS( a, b ) ( ((*(U32*)&(a)) & 0x80000000) == ((*(U32*)&(b)) & 0x80000000) )

    /* Compute a1, b1, c1, where line joining points 1 and 2
    * is "a1 x  +  b1 y  +  c1  =  0".
    */

//PERFINFO_AUTO_START("lines_intersect",1);

    a1 = y2 - y1;
    b1 = x1 - x2;
    c1 = x2 * y1 - x1 * y2;

    /*-- Compute r3 and r4.  --*/
    r3 = a1 * x3 + b1 * y3 + c1;
    r4 = a1 * x4 + b1 * y4 + c1;

    /* Check signs of r3 and r4.  If both point 3 and point 4 lie on
    * same side of line 1, the line segments do not intersect.
    */

    if (r3 != 0 && r4 != 0 && SAME_SIGNS( r3, r4 ))
    {
        //PERFINFO_AUTO_STOP();
        return 0;
    }

    /* Compute a2, b2, c2 */

    a2 = y4 - y3;
    b2 = x3 - x4;
    c2 = x4 * y3 - x3 * y4;

    /* Compute r1 and r2 */

    r1 = a2 * x1 + b2 * y1 + c2;
    r2 = a2 * x2 + b2 * y2 + c2;

    /* Check signs of r1 and r2.  If both point 1 and point 2 lie
    * on same side of second line segment, the line segments do
    * not intersect.
    */

    if ( r1 != 0 && r2 != 0 && SAME_SIGNS( r1, r2 ))
    {
        //PERFINFO_AUTO_STOP();    
        return 0;
    }

    /* Line segments intersect: compute intersection point. */

    denom = a1 * b2 - a2 * b1;
    if ( denom == 0 )
    {
        // collinear lines
        if ( x2 - x1 == 0 )
        {
            if ( y2 < y1 )
                SWAPF32(y2, y1);
            if ( y4 < y3 )
                SWAPF32(y4, y3);

            if ( (y3 < y2 && y3 > y1) || (y4 < y2 && y4 > y1)/* ||
                (y3 <= y2 && y3 >= y1) || (y4 <= y2 && y4 >= y1)*/ )                
            {
                //PERFINFO_AUTO_STOP();
                return 1;
            }
        }
        else
        {
            if ( x2 < x1 )
                SWAPF32(x2, x1);
            if ( x4 < x3 )
                SWAPF32(x4, x3);

            if ( (x3 < x2 && x3 > x1) || (x4 < x2 && x4 > x1) /*||
                (x3 <= x2 && x3 >= x1) || (x4 <= x2 && x4 >= x1)*/ )
            {
                //PERFINFO_AUTO_STOP();
                return 1;
            }
        }
        //PERFINFO_AUTO_STOP();
        return 0;
    }

    /* The denom/2 is to get rounding instead of truncating.  It
    * is added or subtracted to the numerator, depending upon the
    * sign of the numerator.
    */

/*
glong num, offset;
    offset = denom < 0 ? - denom / 2 : denom / 2;
    num = b1 * c2 - b2 * c1;
    *x = ( num < 0 ? num - offset : num + offset ) / denom;
    num = a2 * c1 - a1 * c2;
    *y = ( num < 0 ? num - offset : num + offset ) / denom;
*/

    //PERFINFO_AUTO_STOP();
    return 0;
} /* lines_intersect */


/* a line intersection that doesnt detect collisions on the endpoints and 
    doesnt detect collinear lines 

    from http://www.ggobi.org/docs/api/xlines_8c-source.html */
int lines_cross(F32 ax, F32 ay, F32 bx, F32 by, 
        F32 cx, F32 cy, F32 dx, F32 dy)
{
    /* Check whether line segment [a,b] crosses segment [c,d]. Fast method
    due to Amie Wilkinson. */

    F32 determinant, b1, b2;
            
    bx -= ax;
    by -= ay;
    cx -= ax;
    cy -= ay;
    dx -= ax;
    dy -= ay;
            
    determinant = dx*cy - dy*cx;
                    
    if (determinant == 0.f)
        return 0;
                
    b1 = (cy*bx - cx*by)/determinant;
    if (b1 <= 0.f)
        return 0;
                
    b2 = (dx*by - dy*bx)/determinant;
    if (b2 <= 0.f)
        return 0;
            
    if (b1+b2 <= 1.f)
        return 0;
        
    return 1;
}



//#define lineToLineCollision(p1, p2, p3, p4, dummy) \
//    linelineIntersect((p1)[0], (p1)[1], (p2)[0], (p2)[1], (p3)[0], (p3)[1], (p4)[0], (p4)[1])
#define lineToLineCollision(p1, p2, p3, p4, dummy) \
    lines_intersect((p1)[0], (p1)[1], (p2)[0], (p2)[1], (p3)[0], (p3)[1], (p4)[0], (p4)[1])

int Vec2DCmpFunc(const void *aData, const void *bData)
{
    const Vec2D * const *a = (const Vec2D * const *)aData;
    const Vec2D * const *b = (const Vec2D * const *)bData;
    return (*a)->vec[0] < (*b)->vec[0] ? -1 : ((*a)->vec[0] > (*b)->vec[0] ? 1 : 0);
}

void findCpHorizons( CombinedPoly *cp, Vec2D ***b_horiz, Vec2D ***t_horiz )
{
    int i = 0, j = 0, unique_size, prim_size;
    Vec2D **uniqueVerts;

//PERFINFO_AUTO_START("find unique verts",1);
    eaCreate(&uniqueVerts);
    // find all unique verts
    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        int vertIsUnique[3] = {1,1,1};
        Prim *p = cp->primitives[i];
        for ( j = 0; j < eaSize(&uniqueVerts); ++j )
        {
            if ( p->uv[0][0] == uniqueVerts[j]->vec[0] && p->uv[0][1] == uniqueVerts[j]->vec[1] )
                vertIsUnique[0] = 0;
            if ( p->uv[1][0] == uniqueVerts[j]->vec[0] && p->uv[1][1] == uniqueVerts[j]->vec[1] )
                vertIsUnique[1] = 0;
            if ( p->uv[2][0] == uniqueVerts[j]->vec[0] && p->uv[2][1] == uniqueVerts[j]->vec[1] )
                vertIsUnique[2] = 0;
        }
        if ( vertIsUnique[0] )
        {
            assert((Vec2D*)&p->uv[0]);
            eaPush(&uniqueVerts, (Vec2D*)&p->uv[0]);
        }
        if ( vertIsUnique[1] )
        {
            assert((Vec2D*)&p->uv[1]);
            eaPush(&uniqueVerts, (Vec2D*)&p->uv[1]);
        }
        if ( vertIsUnique[2] )
        {
            assert((Vec2D*)&p->uv[2]);
            eaPush(&uniqueVerts, (Vec2D*)&p->uv[2]);
        }
    }
//PERFINFO_AUTO_STOP();

    unique_size = eaSize(&uniqueVerts);
    prim_size = eaSize(&cp->primitives);

//PERFINFO_AUTO_START("find horizon",1);
    // for each vert, make a vector from vert to +infinity(in theory) and from vert to -infinity(in theory).
    // in actuality, the vectors are from vert to +number bigger than max tri vert and vert to -number smaller than min tri vert 
    for ( i = 0; i < unique_size; ++i )
    {
        //Vec2 *curVert = &uniqueVerts[i]->vec;
        Vec2 *curVert = (Vec2*)uniqueVerts[i];
        int isntBottomHoriz = 0, isntTopHoriz = 0;

        //PERFINFO_AUTO_START("find horizon inner loop",1);
        for ( j = 0; j < prim_size; ++j )
        {
            Prim *p = cp->primitives[j];
            F32 min_x, max_x, min_y, max_y;

            // we have already ruled it out, no need to check against any more polys
            if ( isntBottomHoriz && isntTopHoriz )
                break;

            //PERFINFO_AUTO_START("calc extents",1);
            min_x = MIN(p->uv[0][0], MIN(p->uv[1][0], p->uv[2][0]));
            max_x = MAX(p->uv[0][0], MAX(p->uv[1][0], p->uv[2][0]));
            min_y = MIN(p->uv[0][1], MIN(p->uv[1][1], p->uv[2][1]));
            max_y = MAX(p->uv[0][1], MAX(p->uv[1][1], p->uv[2][1]));
            //PERFINFO_AUTO_STOP();

            if ( (*curVert)[0] <= max_x && (*curVert)[0] >= min_x )
            {
                Vec2 curVert2;
                curVert2[0] = (*curVert)[0];
            //PERFINFO_AUTO_START("top horizon?",1);
                // is this a top horizon?
                if ( (*curVert)[1] >= min_y )
                {
                    curVert2[1] = (*curVert)[1] + (max_y-min_y) + 10.0f;
                    // line - tri collision
                    if ( 
                        (
                            (!Vec2Equals(*curVert,p->uv[0]) && !Vec2Equals(*curVert,p->uv[1])) &&
                            lineToLineCollision(*curVert, curVert2, p->uv[0], p->uv[1], NULL) 
                        ) ||
                        (
                            (!Vec2Equals(*curVert,p->uv[1]) && !Vec2Equals(*curVert,p->uv[2])) &&
                            lineToLineCollision(*curVert, curVert2, p->uv[1], p->uv[2], NULL) 
                        ) ||
                        (
                            (!Vec2Equals(*curVert,p->uv[2]) && !Vec2Equals(*curVert,p->uv[0])) &&
                            lineToLineCollision(*curVert, curVert2, p->uv[2], p->uv[0], NULL) 
                        )
                       )
                        isntTopHoriz = 1;
                }
                else  // it cant be a top horizon, because it intersects this triangle
                    isntTopHoriz = 1;
            //PERFINFO_AUTO_STOP();

            //PERFINFO_AUTO_START("bottom horizon?",1);
                // is this a bottom horizon?
                if ( (*curVert)[1] <= max_y )
                {
                    curVert2[1] = (*curVert)[1] - (max_y-min_y) - 10.0f;
                    // line - tri collision
                    if ( 
                        (
                            (!Vec2Equals(*curVert,p->uv[0]) && !Vec2Equals(*curVert,p->uv[1])) &&
                            lineToLineCollision(*curVert, curVert2, p->uv[0], p->uv[1], NULL) 
                        ) ||
                        (
                            (!Vec2Equals(*curVert,p->uv[1]) && !Vec2Equals(*curVert,p->uv[2])) &&
                            lineToLineCollision(*curVert, curVert2, p->uv[1], p->uv[2], NULL) 
                        ) ||
                        (
                            (!Vec2Equals(*curVert,p->uv[2]) && !Vec2Equals(*curVert,p->uv[0])) &&
                            lineToLineCollision(*curVert, curVert2, p->uv[2], p->uv[0], NULL) 
                        )
                       )
                        isntBottomHoriz = 1;
                }
                else // it cant be a bottom horizon, because it intersects this triangle
                    isntBottomHoriz = 1;
            //PERFINFO_AUTO_STOP();
            }
        }
        //PERFINFO_AUTO_STOP();

        //PERFINFO_AUTO_START("add horizons",1);
        if ( !isntBottomHoriz )
            eaPush(b_horiz, (Vec2D*)curVert);
        if ( !isntTopHoriz )
            eaPush(t_horiz, (Vec2D*)curVert);
        //PERFINFO_AUTO_STOP();
    }
//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("sort horizons",1);
    // sort the horizons by x position
    qsort(*b_horiz, eaSize(b_horiz), sizeof(Vec2D*), Vec2DCmpFunc);
    qsortG(*t_horiz, eaSize(t_horiz), sizeof(Vec2D*), Vec2DCmpFunc);
//PERFINFO_AUTO_STOP();

    eaDestroy(&uniqueVerts);
}

// returns false if pt[0] is not between a[0] and b[0]
__inline int pointIsBelowLine( Vec2 pt, Vec2 a, Vec2 b )
{
    Vec2 ptOnLine;

    // order by x position
    if ( a[0] > b[0] ) swap(&a, &b, sizeof(Vec2));
    
    if ( pt[0] < a[0] || pt[0] > b[0] )
        return 0;

    ptOnLine[0] = pt[0];
    // y = mx + b
    ptOnLine[1] = ((b[1]-a[1])/(b[0]-a[0])) * ptOnLine[0] + a[1];

    return ptOnLine[1] >= pt[1] ? 1 : 0;
}

__inline float distToLine( Vec2 pt, Vec2 a, Vec2 b )
{
    Vec2 ptOnLine;
    float yInt;
    float m;

//PERFINFO_AUTO_START("distToLine",1);
    // order by x position
    if ( a[0] > b[0] ) swap(&a, &b, sizeof(Vec2));
    
    if ( pt[0] < a[0] || pt[0] > b[0] )
    {
        assert("distToLine: pt is not between a and b" == 0);
        return 0;
    }

    ptOnLine[0] = pt[0];
    // y = mx + b
    m = ((b[1]-a[1])/(b[0]-a[0]));
    yInt = a[1] - (m*a[0]);
    ptOnLine[1] = m * ptOnLine[0] + yInt;

//PERFINFO_AUTO_STOP();
    return ptOnLine[1] - pt[1];
}

__inline float distToLineInX( Vec2 pt, Vec2 a, Vec2 b )
{
    swap(&a[0],&a[1], sizeof(F32));
    swap(&b[0],&b[1], sizeof(F32));
    swap(&pt[0],&pt[1], sizeof(F32));
    return distToLine(pt, a, b);
}

__forceinline float InvSqrt(float x)
{float xhalf = 0.5f*x;
int i = *(int*)&x; // get bits for floating value
i = 0x5f3759df - (i>>1); // gives initial guess y0
x = *(float*)&i; // convert bits back to float
x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
x = x*(1.5f-xhalf*x*x);
return x;
}

static __forceinline F32 distToLineAbsolute(Vec2 p, Vec2 lA, Vec2 lB)
{
    F32 dx,dz, nldx,nldz;
    F32 dist,llen,invllen;
    F32 lx, lz;
    Vec2 ld;

    //PERFINFO_AUTO_START("distToLineAbsolute",1);

    subVec2(lB, lA, ld);

    //llen = fsqrt(ld[0]*ld[0] + ld[1]*ld[1]);
    //if (llen == 0.0) { /* point-point dist */
    //    dx = lA[0] - p[0];
    //    dz = lA[1] - p[1];
    //    lx = lA[0]; lz = lA[1];
    //    dist = fsqrt(dx*dx+dz*dz);
    //    PERFINFO_AUTO_STOP();
    //    return dist;
    //}
    //invllen = 1.0 / llen;
    llen = ld[0]*ld[0] + ld[1]*ld[1];
    invllen = InvSqrt(llen);

    dx = p[0] - lA[0];        dz = p[1] - lA[1];
    nldx = ld[0]*invllen; nldz = ld[1]*invllen;
    dist = (dx * nldx + dz * nldz); /* dot product */
    if (dist < 0) {
        lx = lA[0]; lz = lA[1];
    //} else if (dist >= llen) {
    } else if (SQR(dist) >= llen) {
        lx = lA[0] + ld[0]; lz = lA[1] + ld[1];
    } else {
        lx = lA[0] + nldx*dist; lz = lA[1] + nldz*dist;
    }
    dx = p[0] - lx;    dz = p[1] - lz;
    dist = fsqrt(dx*dx + dz*dz);
    //PERFINFO_AUTO_STOP();
    return dist;
}


__inline float pointOnHorizon( float x, Vec2D ***horiz )
{
    int i = 0, horiz_size = eaSize(horiz);
    Vec2 pt, a, b;
    while ( i < horiz_size && (*horiz)[i]->vec[0] < x ) ++i;

    pt[0] = x;
    pt[1] = 0.0f;
    if ( i == 0 )
    {
        memcpy(&a, (*horiz)[i]->vec, sizeof(Vec2));
        memcpy(&b, (*horiz)[i+1]->vec, sizeof(Vec2));
    }
    else
    {
        memcpy(&a, (*horiz)[i-1]->vec, sizeof(Vec2));
        memcpy(&b, (*horiz)[i]->vec, sizeof(Vec2));
    }
    return distToLine( pt, a, b );
}

__inline int nextPointOnHorizon( float x, Vec2D ***horiz )
{
    int i = 0, horiz_size = eaSize(horiz);
    while ( i < horiz_size && (*horiz)[i]->vec[0] < x ) ++i;
    return i;
}

__inline int prevPointOnHorizon( float x, Vec2D ***horiz )
{
    int i = 0, horiz_size = eaSize(horiz);
    while ( i < horiz_size && (*horiz)[i]->vec[0] < x ) ++i;
    return i > 0 ? i - 1 : 0;
}

__inline void nextPrevPointsOnHoriz( float x, Vec2D ***horiz, int *retPrev, int *retNext )
{
    int i = 0, horiz_size = eaSize(horiz);
    while ( i < horiz_size && (*horiz)[i]->vec[0] < x ) ++i;
    if ( retNext )
        *retNext = i;
    if ( retPrev )
        *retPrev = i > 0 ? i - 1 : 0;
}

__inline int minPointOnHoriz( Vec2D ***horiz )
{
    int i = 0, horiz_size = eaSize(horiz), min = 0;
    for ( i = 0; i < horiz_size; ++i )
    {
        if ( (*horiz)[i]->vec[1] < (*horiz)[min]->vec[1] )
            min = i;
    }
    return min;
}

__inline int pointIsAboveHorizon( Vec2 pt, Vec2D ***horiz )
{
    int i = 0, horiz_size = eaSize(horiz);
    Vec2 a, b;
    while ( i < horiz_size && (*horiz)[i]->vec[0] < pt[0] ) ++i;

    if ( i == 0 )
    {
        memcpy(&a, (*horiz)[i]->vec, sizeof(Vec2));
        memcpy(&b, (*horiz)[i+1]->vec, sizeof(Vec2));
    }
    else
    {
        memcpy(&a, (*horiz)[i-1]->vec, sizeof(Vec2));
        memcpy(&b, (*horiz)[i]->vec, sizeof(Vec2));
    }
    return distToLine( pt, a, b ) < 0.0f;
}

int pointsAreTooCloseToHoriz( Vec2D ***points, Vec2D ***horiz, float x_offset, float y_offset )
{
    int i = 0, last = 0, next = 1, points_size = eaSize(points), horiz_size = eaSize(horiz);
    Vec2 newVec;

    //PERFINFO_AUTO_START("pointsAreTooCloseToHoriz",1);

    for ( i = 0; i < points_size-(points_size & 3); /*++i*/ )
    {
#define PATCTH_LOOP last = 0; next = 1; \
        newVec[0] = (*points)[i]->vec[0] + x_offset;\
        newVec[1] = (*points)[i]->vec[1] + y_offset;\
        while ( next < horiz_size )\
        {\
            if ( (*horiz)[next]->vec[0] + NEW_BORDER_SIZE > newVec[0] && (*horiz)[last]->vec[0] - NEW_BORDER_SIZE < newVec[0] ) \
            {\
                if ( distToLineAbsolute(newVec, (*horiz)[last]->vec, (*horiz)[next]->vec) < NEW_BORDER_SIZE )\
                    return 1;\
            }\
            ++last; ++next;\
        }

        PATCTH_LOOP;
        ++i;
        PATCTH_LOOP;
        ++i;
        PATCTH_LOOP;
        ++i;
        PATCTH_LOOP;
        ++i;
    }
    for ( ; i < points_size; ++i )
    {
        PATCTH_LOOP;
    }
    //PERFINFO_AUTO_STOP();
    return 0;
}

int horizIsTooCloseToPoints( Vec2D ***horiz, Vec2D ***points, float x_offset, float y_offset )
{
    int i = 0, last = 0, next = 1, points_size = eaSize(points), horiz_size = eaSize(horiz);
    Vec2 newNext, newLast;

    //PERFINFO_AUTO_START("horizIsTooCloseToPoints",1);

    //for ( i = 0; i < horiz_size; ++i )
    for ( i = 0; i < horiz_size-(horiz_size & 3); /*++i*/ )
    {
#define HITCTP_LOOP last = 0; next = 1;\
        while ( next < points_size )\
        {\
            newLast[0] = (*points)[last]->vec[0] + x_offset;\
            newLast[1] = (*points)[last]->vec[1] + y_offset;\
            newNext[0] = (*points)[next]->vec[0] + x_offset;\
            newNext[1] = (*points)[next]->vec[1] + y_offset;\
            if ( newNext[0] + NEW_BORDER_SIZE > (*horiz)[i]->vec[0] && newLast[0] - NEW_BORDER_SIZE < (*horiz)[i]->vec[0] )\
            {\
                if ( distToLineAbsolute( (*horiz)[i]->vec, newLast, newNext) < NEW_BORDER_SIZE )\
                    return 1;\
            }\
            ++last; ++next;\
        }

        HITCTP_LOOP;
        ++i;
        HITCTP_LOOP;
        ++i;
        HITCTP_LOOP;
        ++i;
        HITCTP_LOOP;
        ++i;
    }

    for ( ; i < horiz_size; ++i )
    {
        HITCTP_LOOP;
    }
    //PERFINFO_AUTO_STOP();
    return 0;
}

int doHorizonsCollide( Vec2D ***cur_horiz, Vec2D ***cp_horiz, float x_offset, float y_offset, float *amount )
{
    int i = 0, last = 0, next = 1, cp_size = eaSize(cp_horiz), saved_last = -1, saved_next = -1;
    Vec2 newPt, newLast, newNext;
    float dist;
    static int id = 0;
//PERFINFO_AUTO_START("doHorizonsCollide",1);

//PERFINFO_AUTO_START("loop 1",1);
    // see if each point from cp_horiz is above cur_horiz
    for ( i = 0; i < cp_size; ++i )
    {
        newPt[0] = (*cp_horiz)[i]->vec[0] + x_offset;
        newPt[1] = (*cp_horiz)[i]->vec[1] + y_offset;
        while ( newPt[0] > (*cur_horiz)[next]->vec[0] )
        {
            ++next;
            if ( next >= eaSize(cur_horiz) )
                goto nexttest;
            ++last; 
        }
        // this is the last point on the horizon that is before the cp
        if (saved_last == -1 )
            saved_last = last;

        dist = distToLine(newPt, (*cur_horiz)[last]->vec, (*cur_horiz)[next]->vec);
        if ( dist >= 0.0f )
        {
            //PERFINFO_AUTO_STOP();
            //PERFINFO_AUTO_STOP();
            *amount = dist;
            return i;
        }
    }
//PERFINFO_AUTO_STOP();

nexttest:

    i = last = 0;
    next = 1;
    while ( i < eaSize(cur_horiz) && (*cur_horiz)[i]->vec[0] < (*cp_horiz)[0]->vec[0] + x_offset ) ++i;

//PERFINFO_AUTO_START("loop 2",1);
    // see if each point from cur_horiz is below cp_horiz
    for ( ; i < eaSize(cur_horiz) && 
        (*cur_horiz)[i]->vec[0] < (*cp_horiz)[eaSize(cp_horiz)-1]->vec[0] + x_offset; ++i )
    {
        newPt[0] = (*cur_horiz)[i]->vec[0];
        newPt[1] = (*cur_horiz)[i]->vec[1];
        while ( newPt[0] > (*cp_horiz)[next]->vec[0] + x_offset )
        {
            ++next;
            if ( next >= eaSize(cp_horiz) )
                goto done;
            ++last; 
        }
        newLast[0] = (*cp_horiz)[last]->vec[0] + x_offset;
        newLast[1] = (*cp_horiz)[last]->vec[1] + y_offset;
        newNext[0] = (*cp_horiz)[next]->vec[0] + x_offset;
        newNext[1] = (*cp_horiz)[next]->vec[1] + y_offset;
        dist = distToLine(newPt, newLast, newNext);
        if ( dist <= 0.0f )
        {
            //PERFINFO_AUTO_STOP();
            //PERFINFO_AUTO_STOP();
            *amount = -dist;
            return i;
        }
    }

//PERFINFO_AUTO_STOP();

done:

    // if the cp is too close to the borders around it it counts as a collision
    if ( pointsAreTooCloseToHoriz(cp_horiz, cur_horiz, x_offset, y_offset) )
    {
        //PERFINFO_AUTO_STOP();
        *amount = 1.0f;
        return 1;
    }
    if ( horizIsTooCloseToPoints(cur_horiz, cp_horiz, x_offset, y_offset) )
    {
        //PERFINFO_AUTO_STOP();
        *amount = 1.0f;
        return 1;
    }

//PERFINFO_AUTO_STOP();

    return -1;
}

float findYOffsetGivenX( Vec2D ***cur_horiz, Vec2D ***cp_horiz, float x_offset, float max_y_offset, float best_y )
{
    float cur_y = 0.0f, dist;

    //PERFINFO_AUTO_START("findYOffsetGivenX",1);

    while ( /*idx = */doHorizonsCollide(cur_horiz, cp_horiz, x_offset, cur_y, &dist) != -1 && cur_y < max_y_offset && cur_y < best_y )
    {
        cur_y += dist + 0.5f;
    }

    //PERFINFO_AUTO_STOP();

    if ( cur_y >= max_y_offset )
        return -1.0f;

    return cur_y;
}

void findCPOffset( Vec2D ***cur_horiz, Vec2D ***cp_horiz, float max, float *x_offset, float *y_offset )
{
    float found_y = 0.0f, found_x = 0.0f,
        best_x = 0.0f, best_y = 999999999.0f;
    int cp_size = eaSize(cp_horiz);

    //PERFINFO_AUTO_START("findCPOffset",1);

    // brute force it, iterate through x positions, incrementing by one, until you get to the end and then keep the best one
    while ( (*cp_horiz)[cp_size-1]->vec[0] + found_x < max )
    {
        found_y = findYOffsetGivenX( cur_horiz, cp_horiz, found_x, max, best_y );

        if ( found_y >= 0.0f && found_y < best_y )
        {
            best_y = found_y;
            best_x = found_x;
        }
        found_x += 1.0f;
    }

    assert ( (*cp_horiz)[cp_size-1]->vec[0] + best_x <= max );

    //PERFINFO_AUTO_STOP();

    if ( best_y > max || best_y < 0.0f )
    {
        *y_offset = -1;
        *x_offset = -1;
        return;
    }

    *x_offset = best_x;
    *y_offset = best_y;
}

void updateCurrentHorizon( Vec2D ***cur_horiz, Vec2D ***cp_horiz, float cp_x_offset, float cp_y_offset )
{
    int i = 0, j = 0, iter = 0, size_cp_horiz = eaSize(cp_horiz), size_cur_horiz = eaSize(cur_horiz);
    Vec2D *cpIntersectHoriz1, *cpIntersectHoriz2;
    float max_x = (*cur_horiz)[size_cur_horiz-1]->vec[0];

    // find the beginning point where the cp horiz will be inserted
    while ( i < size_cur_horiz - 1 && (*cur_horiz)[i]->vec[0] < (*cp_horiz)[0]->vec[0] ) ++i;
    j = i;
    // find the ending point where the cp horiz joins the current one again
    while ( j < size_cur_horiz - 1 && (*cur_horiz)[j]->vec[0] < (*cp_horiz)[size_cp_horiz-1]->vec[0] ) ++j;


    cpIntersectHoriz1 = createSpareVert();
    cpIntersectHoriz2 = createSpareVert();

    assert(eaSize(cp_horiz)-1);


    cpIntersectHoriz1->vec[0] = (*cp_horiz)[0]->vec[0] - 0.001f;
    if ( cpIntersectHoriz1->vec[0] < 0.0f ) 
        cpIntersectHoriz1->vec[0] = 0.0f;
    if ( i > 0 && cpIntersectHoriz1->vec[0] <= (*cur_horiz)[i-1]->vec[0] )
        cpIntersectHoriz1->vec[0] = (*cur_horiz)[i-1]->vec[0] + 0.01f;

    cpIntersectHoriz2->vec[0] = (*cp_horiz)[size_cp_horiz-1]->vec[0] + 0.001f;
    if ( cpIntersectHoriz2->vec[0] > (*cur_horiz)[size_cur_horiz-1]->vec[0] ) 
        cpIntersectHoriz2->vec[0] = (*cur_horiz)[size_cur_horiz-1]->vec[0];
    if ( j < size_cur_horiz && cpIntersectHoriz2->vec[0] >= (*cur_horiz)[j]->vec[0] )
        cpIntersectHoriz1->vec[0] = (*cur_horiz)[j]->vec[0] - 0.01f;
    
    cpIntersectHoriz1->vec[1] = pointOnHorizon( (*cp_horiz)[0]->vec[0], cur_horiz );
    cpIntersectHoriz2->vec[1] = pointOnHorizon( (*cp_horiz)[size_cp_horiz-1]->vec[0], cur_horiz );

    if ( i > 0 && j < size_cur_horiz )
    {
        // delete the verts in between the beginning and ending points
        memmove( &(*cur_horiz)[i], &(*cur_horiz)[j], sizeof(Vec2D*)*(size_cur_horiz - j) );
        eaSetSize(cur_horiz, size_cur_horiz - (j - i));
    }

    // insert the verts of the cp horizon into their proper position in the cur horiz
    for ( iter = size_cp_horiz-1; iter >= 0; --iter )
    {
        eaPush( cur_horiz, (*cp_horiz)[iter] );
    }

    eaPush( cur_horiz, cpIntersectHoriz1 );
    eaPush( cur_horiz, cpIntersectHoriz2 );

    size_cur_horiz = eaSize(cur_horiz);

    // remove duplicate points
    for ( i = 0; i < size_cur_horiz; ++i )
    {    
        for ( j = i + 1; j < size_cur_horiz; ++j )
        {
            // the points have the same x position
            if ( (*cur_horiz)[i]->vec[0] == (*cur_horiz)[j]->vec[0] )
            {
                if ( (*cur_horiz)[i]->vec[1] > (*cur_horiz)[j]->vec[1] )
                {
                    eaRemoveFast(cur_horiz, j);
                    size_cur_horiz = eaSize(cur_horiz);
                }
                else
                {
                    eaRemoveFast(cur_horiz, i);
                    size_cur_horiz = eaSize(cur_horiz);
                }
            }
        }
    }

    qsort(*cur_horiz, eaSize(cur_horiz), sizeof(Vec2D*), Vec2DCmpFunc);
}

static int tryPopulateTextureMap2(CombinedPoly ***cp_list, int try_size)
{
    int i = 0, cpIdx, count = 0, max = 0, cp_count = eaSize(cp_list);
    Vec2D **horiz, **cp_bottom_horiz, **cp_top_horiz;//, *newP = NULL; 
    Vec2 point, point2;

//PERFINFO_AUTO_START("tryPopulateTextureMap2",1);

    // create initial horizon
    eaCreate(&horiz);
    eaCreate(&cp_bottom_horiz);
    eaCreate(&cp_top_horiz);
    eaCreate(&spareVerts);
    point[0] = point[1] = point2[1] = 0.0f;
    eaPush(&horiz, ((Vec2D*)&point));
    point2[0] = try_size;
    eaPush(&horiz, ((Vec2D*)&point2));
    
    for (cpIdx = 0; cpIdx < cp_count; cpIdx++)
    {
        int newIdx = cpIdx;
        //int newIdx = ((cpIdx&1)?(cp_count-((cpIdx+1)>>1)):(cpIdx>>1));
        CombinedPoly *cp = (*cp_list)[newIdx];
        float cp_x_offset = 0.0f, cp_y_offset = 0.0f; // the x position the cp will be fitted into the texture
        
//PERFINFO_AUTO_START("finalize rotation",1);
        // orient the cp so that its height is larger than its width
        if ( !cpOrient(cp, 0) )
            cpFinalizeRotation(cp);
//PERFINFO_AUTO_STOP();

        // TODO: make this not finalize and change the cpSetOffsets below to cpAddOffsets 
        cpNormalizeOffsetsFinalize(cp);

//PERFINFO_AUTO_START("findCpHorizons",1);
        // find the top and bottom horizon of the cp
        eaSetSize(&cp_bottom_horiz, 0);
        eaSetSize(&cp_top_horiz, 0);
        findCpHorizons(cp, &cp_bottom_horiz, &cp_top_horiz);
//PERFINFO_AUTO_STOP();

        // find the best spot for the cp so that there is minimum wasted space between its bottom horizon and the current horizon
        findCPOffset(&horiz, &cp_bottom_horiz, point2[0], &cp_x_offset, &cp_y_offset);

        if ( cp_x_offset < 0 || cp->maxV + cp_y_offset > try_size )
        {
            eaDestroy(&horiz);
            eaDestroy(&cp_bottom_horiz);
            eaDestroy(&cp_top_horiz);
            //PERFINFO_AUTO_STOP();
            return 0;
        }

//PERFINFO_AUTO_START("finalize position",1);
        // TODO: change to cpAddOffsets after changing cpNormalizeOffsetsFinalize to cpNormalizeOffsets above
        cpSetOffsets( cp, cp_x_offset, cp_y_offset );
        cpFinalizePosition( cp );
//PERFINFO_AUTO_STOP();

        if ( 0 )
            drawHorizonsTGA(&cp_top_horiz, &cp_bottom_horiz, &horiz, try_size, "C:\\cps", cpIdx);

//PERFINFO_AUTO_START("updateCurrentHorizon",1);
        // update the horizon with the top horizon of the cp
        updateCurrentHorizon( &horiz, &cp_top_horiz, cp_x_offset, cp_y_offset );
//PERFINFO_AUTO_STOP();
    }

    eaDestroy(&horiz);
    eaDestroy(&cp_bottom_horiz);
    eaDestroy(&cp_top_horiz);
    for ( i = 0; i < eaSize(&spareVerts); ++i )
        free(spareVerts[i]);
    eaDestroy(&spareVerts);
//PERFINFO_AUTO_STOP();
    return 1;
}










//////////////////////////////////////////////////////////////////////////////
// The old way 
//////////////////////////////////////////////////////////////////////////////

//static Rect *findRectForCP(Rect ***rect_list, CombinedPoly *cp)
//{
//    int i, rect_count = eaSize(rect_list);
//    int rect_idx = -1;
//    float min_area = -1;
//    Rect *r;
//    int horiz = cp->widthWithBorder > cp->heightWithBorder;
//
//    for (i = 0; i < rect_count; i++)
//    {
//        float area;
//        int rwidth, rheight;
//        int rhoriz;
//
//        r = (*rect_list)[i];
//        rwidth = rectWidth(r);
//        rheight = rectHeight(r);
//        rhoriz = rwidth > rheight;
//
//        if (horiz == rhoriz)
//        {
//            if (rwidth < cp->widthWithBorder || rheight < cp->heightWithBorder)
//                continue;
//        }
//        else
//        {
//            if (rwidth < cp->heightWithBorder || rheight < cp->widthWithBorder)
//                continue;
//        }
//
//        area = ((float)rwidth) * ((float)rheight);
//        if (min_area < 0 || area < min_area)
//        {
//            min_area = area;
//            rect_idx = i;
//        }
//    }
//
//    if (rect_idx < 0)
//        return 0;
//
//    r = EArrayRemove(rect_list, rect_idx);
//    cpOrient(cp, rectWidth(r) > rectHeight(r));
//    return r;
//}
//
//static void returnRectsToAvailable(CombinedPoly *cp, Rect *use_rect, Rect ***available)
//{
//    int i=0;
//
//    Rect *cp_rect = rectCreate(cp->offsetU, cp->offsetV, cp->widthWithBorder, cp->heightWithBorder);
//
//    rectBooleanSubtract(use_rect, cp_rect, available);
//
//    rectFree(use_rect);
//    rectFree(cp_rect);
//
//#if FILL_HOLES
//    // add CP's available rects
//    for (i = 0; i < eaSize(&cp->availableRects); i++)
//    {
//        Rect *r = rectCopy(cp->availableRects[i]);
//        rectOffset(r, cp->offsetU, cp->offsetV);
//        eaPush(available, r);
//    }
//#endif
//
//    // merge
//    mergeRects(available);
//}
//
//static int tryPopulateTextureMap(CombinedPoly ***cp_list, int try_size)
//{
//    int i = 0, cpIdx, count, cp_count = eaSize(cp_list);
//    int ret = 0;
//    Rect **availableRects = 0;
//
//    eaCreate(&availableRects);
//    eaPush(&availableRects, rectCreate(0, 0, try_size, try_size));
//
//    for (cpIdx = 0; cpIdx < cp_count; cpIdx++)
//    {
//        // get biggest CP
//        CombinedPoly *maxCp = (*cp_list)[cpIdx];
//        Rect *r;
//
//        count = eaSize(&availableRects);
//        if (!count)
//            break;
//
//        // try to find a rectangle that fits the CP
//        r = findRectForCP(&availableRects, maxCp);
//        if (!r)
//            break;
//
//        // map the CP to the rectangle
//        cpSetOffsets(maxCp, r->x, r->y);
//
//        // merge CP's available blocks to the available list
//        returnRectsToAvailable(maxCp, r, &availableRects);
//
//        if (cpIdx == cp_count-1)
//            ret = 1;
//    }
//
//    eaDestroyAll(&availableRects, rectFree);
//
//    return ret;
//}


#endif
