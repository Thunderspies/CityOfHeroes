#include "render/thread/ogl.h"
#include <utilitieslib/assert/assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/memcheck.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

/*
* Compute ceiling of integer quotient of A divided by B:
*/
#define CEILING( A, B )  ( (A) % (B) == 0 ? (A)/(B) : (A)/(B)+1 )



#ifdef EPSILON
#undef EPSILON
#endif
#define EPSILON 0.001


/* To work around optimizer bug in MSVC4.1 */
#if defined(__WIN32__) && !defined(OPENSTEP)
void dummy(GLuint j, GLuint k){
}
#else
#define dummy(J, K)
#endif


static GLint APIENTRY gluScaleImageX( GLenum format,
                                     GLint widthin, GLint heightin,
                                     GLenum typein, const void *datain,
                                     GLint widthout, GLint heightout,
                                     GLenum typeout, void *dataout )
{
    GLint components, i, j, k;
    GLfloat *tempin, *tempout;
    GLfloat sx, sy;
    GLint unpackrowlength, unpackalignment, unpackskiprows, unpackskippixels;
    GLint packrowlength, packalignment, packskiprows, packskippixels;
    GLint sizein, sizeout;
    GLint rowstride, rowlen;


    /* Determine number of components per pixel */
    switch (format) {
case GL_COLOR_INDEX:
case GL_STENCIL_INDEX:
case GL_DEPTH_COMPONENT:
case GL_RED:
case GL_GREEN:
case GL_BLUE:
case GL_ALPHA:
case GL_LUMINANCE:
    components = 1;
    break;
case GL_LUMINANCE_ALPHA:
    components = 2;
    break;
case GL_RGB:
    components = 3;
    break;
case GL_RGBA:
    components = 4;
    break;
default:
    return GLU_INVALID_ENUM;
    }

    /* Determine bytes per input datum */
    switch (typein) {
case GL_UNSIGNED_BYTE:    sizein = sizeof(GLubyte);    break;
case GL_BYTE:        sizein = sizeof(GLbyte);    break;
case GL_UNSIGNED_SHORT:    sizein = sizeof(GLushort);    break;
case GL_SHORT:        sizein = sizeof(GLshort);    break;
case GL_UNSIGNED_INT:    sizein = sizeof(GLuint);    break;
case GL_INT:        sizein = sizeof(GLint);        break;
case GL_FLOAT:        sizein = sizeof(GLfloat);    break;
case GL_BITMAP:
    /* not implemented yet */
default:
    return GL_INVALID_ENUM;
    }

    /* Determine bytes per output datum */
    switch (typeout) {
case GL_UNSIGNED_BYTE:    sizeout = sizeof(GLubyte);    break;
case GL_BYTE:        sizeout = sizeof(GLbyte);    break;
case GL_UNSIGNED_SHORT:    sizeout = sizeof(GLushort);    break;
case GL_SHORT:        sizeout = sizeof(GLshort);    break;
case GL_UNSIGNED_INT:    sizeout = sizeof(GLuint);    break;
case GL_INT:        sizeout = sizeof(GLint);    break;
case GL_FLOAT:        sizeout = sizeof(GLfloat);    break;
case GL_BITMAP:
    /* not implemented yet */
default:
    return GL_INVALID_ENUM;
    }

    /* Get glPixelStore state */
    glGetIntegerv( GL_UNPACK_ROW_LENGTH, &unpackrowlength );
    glGetIntegerv( GL_UNPACK_ALIGNMENT, &unpackalignment );
    glGetIntegerv( GL_UNPACK_SKIP_ROWS, &unpackskiprows );
    glGetIntegerv( GL_UNPACK_SKIP_PIXELS, &unpackskippixels );
    glGetIntegerv( GL_PACK_ROW_LENGTH, &packrowlength );
    glGetIntegerv( GL_PACK_ALIGNMENT, &packalignment );
    glGetIntegerv( GL_PACK_SKIP_ROWS, &packskiprows );
    glGetIntegerv( GL_PACK_SKIP_PIXELS, &packskippixels );

    /* Allocate storage for intermediate images */
    tempin = (GLfloat *) calloc( widthin * heightin
        * components, sizeof(GLfloat));
    if (!tempin) {
        return GLU_OUT_OF_MEMORY;
    }
    tempout = (GLfloat *) calloc( widthout * heightout
        * components, sizeof(GLfloat));
    if (!tempout) {
        free( tempin );
        return GLU_OUT_OF_MEMORY;
    }


    /*
    * Unpack the pixel data and convert to floating point
    */

    if (unpackrowlength>0) {
        rowlen = unpackrowlength;
    }
    else {
        rowlen = widthin;
    }
    if (sizein >= unpackalignment) {
        rowstride = components * rowlen;
    }
    else {
        rowstride = unpackalignment/sizein
            * CEILING( components * rowlen * sizein, unpackalignment );
    }

    switch (typein) {
case GL_UNSIGNED_BYTE:
    k = 0;
    for (i=0;i<heightin;i++) {
        GLubyte *ubptr = (GLubyte *) datain
            + i * rowstride
            + unpackskiprows * rowstride
            + unpackskippixels * components;
        for (j=0;j<widthin*components;j++) {
            dummy(j, k);
            tempin[k++] = (GLfloat) *ubptr++;
        }
    }
    break;
case GL_BYTE:
    k = 0;
    for (i=0;i<heightin;i++) {
        GLbyte *bptr = (GLbyte *) datain
            + i * rowstride
            + unpackskiprows * rowstride
            + unpackskippixels * components;
        for (j=0;j<widthin*components;j++) {
            dummy(j, k);
            tempin[k++] = (GLfloat) *bptr++;
        }
    }
    break;
case GL_UNSIGNED_SHORT:
    k = 0;
    for (i=0;i<heightin;i++) {
        GLushort *usptr = (GLushort *) datain
            + i * rowstride
            + unpackskiprows * rowstride
            + unpackskippixels * components;
        for (j=0;j<widthin*components;j++) {
            dummy(j, k);
            tempin[k++] = (GLfloat) *usptr++;
        }
    }
    break;
case GL_SHORT:
    k = 0;
    for (i=0;i<heightin;i++) {
        GLshort *sptr = (GLshort *) datain
            + i * rowstride
            + unpackskiprows * rowstride
            + unpackskippixels * components;
        for (j=0;j<widthin*components;j++) {
            dummy(j, k);
            tempin[k++] = (GLfloat) *sptr++;
        }
    }
    break;
case GL_UNSIGNED_INT:
    k = 0;
    for (i=0;i<heightin;i++) {
        GLuint *uiptr = (GLuint *) datain
            + i * rowstride
            + unpackskiprows * rowstride
            + unpackskippixels * components;
        for (j=0;j<widthin*components;j++) {
            dummy(j, k);
            tempin[k++] = (GLfloat) *uiptr++;
        }
    }
    break;
case GL_INT:
    k = 0;
    for (i=0;i<heightin;i++) {
        GLint *iptr = (GLint *) datain
            + i * rowstride
            + unpackskiprows * rowstride
            + unpackskippixels * components;
        for (j=0;j<widthin*components;j++) {
            dummy(j, k);
            tempin[k++] = (GLfloat) *iptr++;
        }
    }
    break;
case GL_FLOAT:
    k = 0;
    for (i=0;i<heightin;i++) {
        GLfloat *fptr = (GLfloat *) datain
            + i * rowstride
            + unpackskiprows * rowstride
            + unpackskippixels * components;
        for (j=0;j<widthin*components;j++) {
            dummy(j, k);
            tempin[k++] = *fptr++;
        }
    }
    break;
default:
    return GLU_INVALID_ENUM;
    }


    /*
    * Scale the image!
    */

    if (widthout > 1)
        sx = (GLfloat) (widthin-1) / (GLfloat) (widthout-1);
    else
        sx = (GLfloat) (widthin-1);
    if (heightout > 1)
        sy = (GLfloat) (heightin-1) / (GLfloat) (heightout-1);
    else
        sy = (GLfloat) (heightin-1);

    /*#define POINT_SAMPLE*/
#ifdef POINT_SAMPLE
    for (i=0;i<heightout;i++) {
        GLint ii = i * sy;
        for (j=0;j<widthout;j++) {
            GLint jj = j * sx;

            GLfloat *src = tempin + (ii * widthin + jj) * components;
            GLfloat *dst = tempout + (i * widthout + j) * components;

            for (k=0;k<components;k++) {
                *dst++ = *src++;
            }
        }
    }
#else
    if (sx<1.0 && sy<1.0) {
        /* magnify both width and height:  use weighted sample of 4 pixels */
        GLint i0, i1, j0, j1;
        GLfloat alpha, beta;
        GLfloat *src00, *src01, *src10, *src11;
        GLfloat s1, s2;
        GLfloat *dst;

        for (i=0;i<heightout;i++) {
            i0 = i * sy;
            i1 = i0 + 1;
            if (i1 >= heightin) i1 = heightin-1;
            /*     i1 = (i+1) * sy - EPSILON;*/
            alpha = i*sy - i0;
            for (j=0;j<widthout;j++) {
                j0 = j * sx;
                j1 = j0 + 1;
                if (j1 >= widthin) j1 = widthin-1;
                /*        j1 = (j+1) * sx - EPSILON; */
                beta = j*sx - j0;

                /* compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
                src00 = tempin + (i0 * widthin + j0) * components;
                src01 = tempin + (i0 * widthin + j1) * components;
                src10 = tempin + (i1 * widthin + j0) * components;
                src11 = tempin + (i1 * widthin + j1) * components;

                dst = tempout + (i * widthout + j) * components;

                for (k=0;k<components;k++) {
                    s1 = *src00++ * (1.0-beta) + *src01++ * beta;
                    s2 = *src10++ * (1.0-beta) + *src11++ * beta;
                    *dst++ = s1 * (1.0-alpha) + s2 * alpha;
                }
            }
        }
    }
    else {
        /* shrink width and/or height:  use an unweighted box filter */
        GLint i0, i1;
        GLint j0, j1;
        GLint ii, jj;
        GLfloat sum, *dst;

        for (i=0;i<heightout;i++) {
            i0 = i * sy;
            i1 = i0 + 1;
            if (i1 >= heightin) i1 = heightin-1; 
            /*     i1 = (i+1) * sy - EPSILON; */
            for (j=0;j<widthout;j++) {
                j0 = j * sx;
                j1 = j0 + 1;
                if (j1 >= widthin) j1 = widthin-1;
                /*        j1 = (j+1) * sx - EPSILON; */

                dst = tempout + (i * widthout + j) * components;

                /* compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
                for (k=0;k<components;k++) {
                    sum = 0.0;
                    for (ii=i0;ii<=i1;ii++) {
                        for (jj=j0;jj<=j1;jj++) {
                            sum += *(tempin + (ii * widthin + jj) * components + k);
                        }
                    }
                    sum /= (j1-j0+1) * (i1-i0+1);
                    *dst++ = sum;
                }
            }
        }
    }
#endif


    /*
    * Return output image
    */

    if (packrowlength>0) {
        rowlen = packrowlength;
    }
    else {
        rowlen = widthout;
    }
    if (sizeout >= packalignment) {
        rowstride = components * rowlen;
    }
    else {
        rowstride = packalignment/sizeout
            * CEILING( components * rowlen * sizeout, packalignment );
    }

    switch (typeout) {
case GL_UNSIGNED_BYTE:
    k = 0;
    for (i=0;i<heightout;i++) {
        GLubyte *ubptr = (GLubyte *) dataout
            + i * rowstride
            + packskiprows * rowstride
            + packskippixels * components;
        for (j=0;j<widthout*components;j++) {
            dummy(j, k+i);
            *ubptr++ = (GLubyte) tempout[k++];
        }
    }
    break;
case GL_BYTE:
    k = 0;
    for (i=0;i<heightout;i++) {
        GLbyte *bptr = (GLbyte *) dataout
            + i * rowstride
            + packskiprows * rowstride
            + packskippixels * components;
        for (j=0;j<widthout*components;j++) {
            dummy(j, k+i);
            *bptr++ = (GLbyte) tempout[k++];
        }
    }
    break;
case GL_UNSIGNED_SHORT:
    k = 0;
    for (i=0;i<heightout;i++) {
        GLushort *usptr = (GLushort *) dataout
            + i * rowstride
            + packskiprows * rowstride
            + packskippixels * components;
        for (j=0;j<widthout*components;j++) {
            dummy(j, k+i);
            *usptr++ = (GLushort) tempout[k++];
        }
    }
    break;
case GL_SHORT:
    k = 0;
    for (i=0;i<heightout;i++) {
        GLshort *sptr = (GLshort *) dataout
            + i * rowstride
            + packskiprows * rowstride
            + packskippixels * components;
        for (j=0;j<widthout*components;j++) {
            dummy(j, k+i);
            *sptr++ = (GLshort) tempout[k++];
        }
    }
    break;
case GL_UNSIGNED_INT:
    k = 0;
    for (i=0;i<heightout;i++) {
        GLuint *uiptr = (GLuint *) dataout
            + i * rowstride
            + packskiprows * rowstride
            + packskippixels * components;
        for (j=0;j<widthout*components;j++) {
            dummy(j, k+i);
            *uiptr++ = (GLuint) tempout[k++];
        }
    }
    break;
case GL_INT:
    k = 0;
    for (i=0;i<heightout;i++) {
        GLint *iptr = (GLint *) dataout
            + i * rowstride
            + packskiprows * rowstride
            + packskippixels * components;
        for (j=0;j<widthout*components;j++) {
            dummy(j, k+i);
            *iptr++ = (GLint) tempout[k++];
        }
    }
    break;
case GL_FLOAT:
    k = 0;
    for (i=0;i<heightout;i++) {
        GLfloat *fptr = (GLfloat *) dataout
            + i * rowstride
            + packskiprows * rowstride
            + packskippixels * components;
        for (j=0;j<widthout*components;j++) {
            dummy(j, k+i);
            *fptr++ = tempout[k++];
        }
    }
    break;
default:
    return GLU_INVALID_ENUM;
    }


    /* free temporary image storage */
    free( tempin );
    free( tempout );

    return 0;
}



/*
* Return the largest k such that 2^k <= n.
*/
static GLint ilog2( GLint n )
{
    GLint k;

    if (n<=0) return 0;
    for (k=0; n>>=1; k++) ;
    return k;
}



/*
* Find the value nearest to n which is also a power of two.
*/
static GLint round2( GLint n )
{
    GLint m;

    for (m=1; m<n; m*=2)
        ;

    /* m>=n */
    if (m-n <= n-m/2) {
        return m;
    }
    else {
        return m/2;
    }
}


/*
* Given an pixel format and datatype, return the number of bytes to
* store one pixel.
*/
static GLint bytes_per_pixel( GLenum format, GLenum type )
{
    GLint n, m;

    switch (format) {
case GL_COLOR_INDEX:
case GL_STENCIL_INDEX:
case GL_DEPTH_COMPONENT:
case GL_RED:
case GL_GREEN:
case GL_BLUE:
case GL_ALPHA:
case GL_LUMINANCE:
    n = 1;
    break;
case GL_LUMINANCE_ALPHA:
    n = 2;
    break;
case GL_RGB:
    n = 3;
    break;
case GL_RGBA:
    n = 4;
    break;
default:
    n = 0;
    }

    switch (type) {
case GL_UNSIGNED_BYTE:    m = sizeof(GLubyte);    break;
case GL_BYTE:        m = sizeof(GLbyte);    break;
case GL_BITMAP:        m = 1;            break;
case GL_UNSIGNED_SHORT:    m = sizeof(GLushort);    break;
case GL_SHORT:        m = sizeof(GLshort);    break;
case GL_UNSIGNED_INT:    m = sizeof(GLuint);    break;
case GL_INT:        m = sizeof(GLint);    break;
case GL_FLOAT:        m = sizeof(GLfloat);    break;
default:            m = 0;
    }

    return n * m;
}

#if 1
void quantTex(void *data,int w,int h);

static GLint APIENTRY gluScaleImage( GLenum format,
                                    GLint w, GLint h,
                                    GLenum typein, const U32 *datain,
                                    GLint neww, GLint newh,
                                    GLenum typeout, U32 *dataout )
{
    int        i,x,y,y_in[2];
    U32        pixel,rgba[4],*pix;

    if (neww == w && newh == h)
    {
        memcpy(dataout,datain,w*h*4);
        return 0;
    }
    if ((neww<<1) != w || (newh<<1) != h)
        return gluScaleImageX(format,w,h,typein,datain,neww,newh,typeout,dataout);

    for(pix=dataout,y=0;y<newh;y++)
    {
        y_in[0] = (y*2+0) * w;
        y_in[1] = (y*2+1) * w;
        for(x=0;x<neww;x++)
        {
            for(i=0;i<4;i++)
                rgba[i] = 0;
            for(i=0;i<4;i++)
            {
                pixel = datain[y_in[i>>1] + (x<<1) + (i&1)];
                rgba[0] += (pixel >> (8 * 0)) & 255;
                rgba[1] += (pixel >> (8 * 1)) & 255;
                rgba[2] += (pixel >> (8 * 2)) & 255;
                rgba[3] += (pixel >> (8 * 3)) & 255;
            }
            *pix++ = (rgba[0] >> 2) << (8 * 0)
                | (rgba[1] >> 2) << (8 * 1)
                | (rgba[2] >> 2) << (8 * 2)
                | (rgba[3] >> 2) << (8 * 3);
        }
    }
    return 0;
}
#endif

GLint COH_Build2DMipmapsSRC(GLenum target, GLenum dst_format,
                            GLint width, GLint height, GLenum src_format,
                            GLenum type, const void *data)
{
    GLint w, h, maxsize;
    void *image, *newimage;
    GLint neww, newh, level, bpp;
    int error;

    if (width < 1 || height < 1)
        return GLU_INVALID_VALUE;

    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxsize );

    w = round2( width );
    if (w>maxsize) {
        w = maxsize;
    }
    h = round2( height );
    if (h>maxsize) {
        h = maxsize;
    }

    bpp = bytes_per_pixel( src_format, type );
    if (bpp==0) {
        /* probably a bad format or type enum */
        return GLU_INVALID_ENUM;
    }

    if (w!=width || h!=height) {
        /* must rescale image to get "top" mipmap texture image */
        image = calloc((w+4) * h * bpp, 1);
        if (!image) {
            return GLU_OUT_OF_MEMORY;
        }
        error = gluScaleImage( src_format, width, height, type, data,
            w, h, type, image );
        if (error) {
            return error;
        }
    }
    else {
        image = (void *) data;
    }

    level = 0;
    for(;;)
    {
        glTexImage2D( target, level, dst_format, w, h, 0, src_format, type, image );
        if (w==1 && h==1)  break;

        neww = (w<2) ? 1 : w/2;
        newh = (h<2) ? 1 : h/2;
        newimage = calloc((neww+4) * newh * bpp, 1);
        if (!newimage) {
            return GLU_OUT_OF_MEMORY;
        }

        error =  gluScaleImage( src_format, w, h, type, image,
            neww, newh, type, newimage );
        if (error) {
            return error;
        }

        if (image!=data) {
            free( image );
        }
        image = newimage;

        w = neww;
        h = newh;
        level++;
    }

    if (image!=data) {
        free( image );
    }

    return 0;
}

