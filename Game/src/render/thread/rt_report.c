// This file is based on visualinfo.c from GLEW which is under MIT license.

/*
** visualinfo.c
**
** Copyright (C) Nate Robins, 1997
**               Michael Wimmer, 1999
**               Milan Ikits, 2002-2008
**
** visualinfo is a small utility that displays all available visuals,
** aka. pixelformats, in an OpenGL system along with renderer version
** information. It shows a table of all the visuals that support OpenGL
** along with their capabilities. The format of the table is similar to
** that of glxinfo on Unix systems:
**
** visual ~= pixel format descriptor
** id       = visual id (integer from 1 - max visuals)
** tp       = type (wn: window, pb: pbuffer, wp: window & pbuffer, bm: bitmap)
** ac        = acceleration (ge: generic, fu: full, no: none)
** fm        = format (i: integer, f: float, c: color index)
** db        = double buffer (y = yes)
** sw       = swap method (x: exchange, c: copy, u: undefined)
** st        = stereo (y = yes)
** sz       = total # bits
** r        = # bits of red
** g        = # bits of green
** b        = # bits of blue
** a        = # bits of alpha
** axbf     = # aux buffers
** dpth     = # bits of depth
** stcl     = # bits of stencil
*/

#include "render/thread/ogl.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/osdependent.h>

static void VisualInfo(HDC dc);
static void PrintExtensions(const char* s);
static void PrintHypnos(const char* s);
static void PrintAdapters(void);

static FILE* file = 0;

const char *makeReportFile()
{
    static char fileName[MAX_PATH] = "";

    /* ---------------------------------------------------------------------- */
    /* open file */
    //strcpy_s(fileName, sizeof(fileName), fileTempName());
    // Use a fixed file name instead of a generated temporary name so the old files don't accumulate in the temp folder
    if (GetTempPathA(MAX_PATH, fileName))
        strcat_s(fileName, MAX_PATH, "opengl.txt");

    file = fopen(fileName, "w");
    if (file == NULL)
        return NULL;

    /* ---------------------------------------------------------------------- */
    /* output header information */
    /* OpenGL extensions */
    //fprintf(file, "OpenGL vendor string: %s\n", glGetString(GL_VENDOR));
    //fprintf(file, "OpenGL renderer string: %s\n", glGetString(GL_RENDERER));
    //fprintf(file, "OpenGL version string: %s\n", glGetString(GL_VERSION));
    fprintf(file, "OpenGL extensions (GL_): \n");
    PrintExtensions((const char*)glGetString(GL_EXTENSIONS));
    PrintHypnos((const char*)glGetString(GL_EXTENSIONS));
    /* GLU extensions */
    fprintf(file, "GLU version string: %s\n", gluGetString(GLU_VERSION));
    fprintf(file, "GLU extensions (GLU_): \n");
    PrintExtensions((const char*)gluGetString(GLU_EXTENSIONS));

    /* ---------------------------------------------------------------------- */
    /* extensions string */
    if (WGLEW_ARB_extensions_string || WGLEW_EXT_extensions_string)
    {
        fprintf(file, "WGL extensions (WGL_): \n");
        PrintExtensions(wglGetExtensionsStringARB ? 
            (char*)wglGetExtensionsStringARB(wglGetCurrentDC()) :
        (char*)wglGetExtensionsStringEXT());
    }

    /* ---------------------------------------------------------------------- */
    /* enumerate all the formats */
    if(!IsUsingCider()) {
        // this call has issues on mac currently, transgaming needs to fix their implementation of wglGetPixelFormatARB
        VisualInfo(wglGetCurrentDC());
    }

    /* ---------------------------------------------------------------------- */
    /* enumerate all the display adapters */
    PrintAdapters();

    /* ---------------------------------------------------------------------- */
    /* release resources */
    fclose(file);
    return fileName;
}

static void PrintHypnos(const char* s)
{
    int max_texunits;
    int max_texcoods;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &max_texunits);
    glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &max_texcoods);
    fprintf(file, "GL_MAX_TEXTURE_IMAGE_UNITS_ARB = %d\n", max_texunits);
    fprintf(file, "GL_MAX_TEXTURE_COORDS_ARB = %d\n", max_texcoods);

    // Shader program limits
    if (strstr(s, "GL_ARB_vertex_program") &&
        strstr(s, "GL_ARB_fragment_program"))
    {
        int max_program_local_parameters;
        int max_program_env_parameters;
        int max_program_native_attribs;
        int max_program_native_instructions;
        int max_program_native_temporaries;
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &max_program_local_parameters);
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &max_program_env_parameters);
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &max_program_native_attribs);
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &max_program_native_instructions);
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &max_program_native_temporaries);
        fprintf(file, "GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB = %d\n", max_program_local_parameters);
        fprintf(file, "GL_MAX_PROGRAM_ENV_PARAMETERS_ARB = %d\n", max_program_env_parameters);
        fprintf(file, "GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB = %d\n", max_program_native_attribs);
        fprintf(file, "GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB = %d\n", max_program_native_instructions);
        fprintf(file, "GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB = %d\n", max_program_native_temporaries);
    }
    if (strstr(s, "GL_EXT_texture_filter_anisotropic"))
    {
        float largest_supported_anisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
        fprintf(file, "GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT = %f\n", largest_supported_anisotropy);
    }
    if (strstr(s, "GL_EXT_framebuffer_multisample"))
    {
        int max_fbo_samples = 0;
        glGetIntegerv(GL_MAX_SAMPLES_EXT, &max_fbo_samples);
        fprintf(file, "GL_MAX_SAMPLES_EXT = %d\n", max_fbo_samples);
    }
    if (strstr(s, "GL_EXT_framebuffer_object"))
    {
        int mac_renderbuffer_size = 0;
        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &mac_renderbuffer_size);
        fprintf(file, "GL_MAX_RENDERBUFFER_SIZE_EXT = %d\n", mac_renderbuffer_size);
    }
}

/* do the magic to separate all extensions with comma's, except
for the last one that _may_ terminate in a space. */
static void PrintExtensions(const char* s)
{
    char t[80];
    int i=0;
    char* p=0;

    t[79] = '\0';
    while (*s)
    {
        t[i++] = *s;
        if(*s == ' ')
        {
            if (*(s+1) != '\0') {
                t[i-1] = ',';
                t[i] = ' ';
                p = &t[i++];
            }
            else /* zoinks! last one terminated in a space! */
            {
                t[i-1] = '\0';
            }
        }
        if(i > 80 - 5)
        {
            *p = t[i] = '\0';
            fprintf(file, "    %s\n", t);
            p++;
            i = (int)strlen(p);
            strcpy(t, p);
        }
        s++;
    }
    t[i] = '\0';
    fprintf(file, "    %s.\n", t);
}

// log the display adapters on the system which can be useful when
// trying to sort out if game is not using settings from the correct adapter
static void PrintAdapters(void)
{
    int i = 0;
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    while (EnumDisplayDevices(NULL, i++, &dd, 0))
    {
        fprintf(file, "Display Device: %d\n", i);
        fprintf(file, "\tDevice Name : %s\n", dd.DeviceName);
        fprintf(file, "\tDevice String: %s\n", dd.DeviceString);
        fprintf(file, "\tDevice ID: %s\n", dd.DeviceID);
        fprintf(file, "\tDevice Key: %s\n", dd.DeviceKey);
        //This string has the device vendor and device information
        //e.g. "PCI\VEN_1002&DEV_68B8&SUBSYS_25431002&REV_00"
        //See vendor sites or http://www.pcidatabase.com/ for vendor and device IDs
        fprintf(file, "\tDevice Flags: 0x%X ",dd.StateFlags);
        // append set device flag bits in English
        if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE)
            fprintf(file, "ACTIVE ");
        if (dd.StateFlags & DISPLAY_DEVICE_MULTI_DRIVER)
            fprintf(file, "MULTI ");
        if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
            fprintf(file, "PRIMARY ");
        if (dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
            fprintf(file, "MIRROR ");
        if (dd.StateFlags & DISPLAY_DEVICE_VGA_COMPATIBLE)
            fprintf(file, "VGA ");
        if (dd.StateFlags & DISPLAY_DEVICE_REMOVABLE)
            fprintf(file, "REMOVABLE ");
        if (dd.StateFlags & DISPLAY_DEVICE_MODESPRUNED)
            fprintf(file, "MODESPRUNED ");
        if (dd.StateFlags & DISPLAY_DEVICE_REMOTE)
            fprintf(file, "REMOTE ");
        if (dd.StateFlags & DISPLAY_DEVICE_DISCONNECT)
            fprintf(file, "DISCONNECT ");
        fprintf(file, "\n");
    }
}

static void VisualInfoARB(HDC dc)
{
    int attrib[32], value[32], n_attrib, n_pbuffer=0, n_float=0;
    int i, pf, maxpf;
    unsigned int c;

    /* to get pbuffer capable pixel formats */
    attrib[0] = WGL_DRAW_TO_PBUFFER_ARB;
    attrib[1] = GL_TRUE;
    attrib[2] = 0;
    wglChoosePixelFormatARB(dc, attrib, 0, 1, &pf, &c);
    /* query number of pixel formats */
    attrib[0] = WGL_NUMBER_PIXEL_FORMATS_ARB;
    for (i=0; i<32; i++) value[i] = 0; // clear in case the call fails
    wglGetPixelFormatAttribivARB(dc, 0, 0, 1, attrib, value);
    maxpf = value[0];
    for (i=0; i<32; i++) value[i] = 0; // clear since we got our man

    attrib[0] = WGL_SUPPORT_OPENGL_ARB;
    attrib[1] = WGL_DRAW_TO_WINDOW_ARB;
    attrib[2] = WGL_DRAW_TO_BITMAP_ARB;
    attrib[3] = WGL_ACCELERATION_ARB;
    /* WGL_NO_ACCELERATION_ARB, WGL_GENERIC_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB */
    attrib[4] = WGL_SWAP_METHOD_ARB;
    /* WGL_SWAP_EXCHANGE_ARB, WGL_SWAP_COPY_ARB, WGL_SWAP_UNDEFINED_ARB */
    attrib[5] = WGL_DOUBLE_BUFFER_ARB;
    attrib[6] = WGL_STEREO_ARB;
    attrib[7] = WGL_PIXEL_TYPE_ARB;
    /* WGL_TYPE_RGBA_ARB, WGL_TYPE_COLORINDEX_ARB,
    WGL_TYPE_RGBA_FLOAT_ATI (WGL_ATI_pixel_format_float) */
    /* Color buffer information */
    attrib[8] = WGL_COLOR_BITS_ARB;
    attrib[9] = WGL_RED_BITS_ARB;
    attrib[10] = WGL_GREEN_BITS_ARB;
    attrib[11] = WGL_BLUE_BITS_ARB;
    attrib[12] = WGL_ALPHA_BITS_ARB;
    /* Accumulation buffer information */
    attrib[13] = WGL_ACCUM_BITS_ARB;
    attrib[14] = WGL_ACCUM_RED_BITS_ARB;
    attrib[15] = WGL_ACCUM_GREEN_BITS_ARB;
    attrib[16] = WGL_ACCUM_BLUE_BITS_ARB;
    attrib[17] = WGL_ACCUM_ALPHA_BITS_ARB;
    /* Depth, stencil, and aux buffer information */
    attrib[18] = WGL_DEPTH_BITS_ARB;
    attrib[19] = WGL_STENCIL_BITS_ARB;
    attrib[20] = WGL_AUX_BUFFERS_ARB;
    /* Layer information */
    attrib[21] = WGL_NUMBER_OVERLAYS_ARB;
    attrib[22] = WGL_NUMBER_UNDERLAYS_ARB;
    attrib[23] = WGL_SWAP_LAYER_BUFFERS_ARB;
    attrib[24] = WGL_SAMPLES_ARB;
    attrib[25] = WGL_SUPPORT_GDI_ARB;
    n_attrib = 26;
    if (WGLEW_ARB_pbuffer)
    {
        attrib[n_attrib] = WGL_DRAW_TO_PBUFFER_ARB;
        n_pbuffer = n_attrib;
        n_attrib++;
    }
    if (WGLEW_NV_float_buffer)
    {
        attrib[n_attrib] = WGL_FLOAT_COMPONENTS_NV;
        n_float = n_attrib;
        n_attrib++;
    }

    /* print table header */
    fprintf(file, " +-----+-------------------------+-----------------+----------+-----------------+----------+\n");
    fprintf(file, " |     |          visual         |      color      | ax dp st |      accum      |   layer  |\n");
    fprintf(file, " |  id | tp ac gd fm db sw st ms |  sz  r  g  b  a | bf th cl |  sz  r  g  b  a | ov un sw |\n");
    fprintf(file, " +-----+-------------------------+-----------------+----------+-----------------+----------+\n");
    /* loop through all the pixel formats */
    for(i = 1; i <= maxpf; i++)
    {
        wglGetPixelFormatAttribivARB(dc, i, 0, n_attrib, attrib, value);
        /* only describe this format if it supports OpenGL */
        if (!value[0]) continue;
        /* print out the information for this visual */
        /* visual id */
        fprintf(file, " |% 4d | ", i);
        /* visual type */
        if (value[1])
        {
            if (WGLEW_ARB_pbuffer && value[n_pbuffer]) fprintf(file, "wp ");
            else fprintf(file, "wn ");
        }
        else
        {
            if (value[2]) fprintf(file, "bm ");
            else if (WGLEW_ARB_pbuffer && value[n_pbuffer]) fprintf(file, "pb ");
        }
        /* acceleration */
        fprintf(file, "%s ", value[3] == WGL_FULL_ACCELERATION_ARB ? "fu" : 
            value[3] == WGL_GENERIC_ACCELERATION_ARB ? "ge" :
            value[3] == WGL_NO_ACCELERATION_ARB ? "no" : ". ");
        /* gdi support */
        fprintf(file, " %c ", value[25] ? 'y' : '.');
        /* format */
        if (WGLEW_NV_float_buffer && value[n_float]) fprintf(file, " f ");
        else if (WGLEW_ATI_pixel_format_float && value[7] == WGL_TYPE_RGBA_FLOAT_ATI) fprintf(file, " f ");
        else if (value[7] == WGL_TYPE_RGBA_ARB) fprintf(file, " i ");
        else if (value[7] == WGL_TYPE_COLORINDEX_ARB) fprintf(file, " c ");
        else fprintf(file, " . ");
        /* double buffer */
        fprintf(file, " %c ", value[5] ? 'y' : '.');
        /* swap method */
        if (value[4] == WGL_SWAP_EXCHANGE_ARB) fprintf(file, " x ");
        else if (value[4] == WGL_SWAP_COPY_ARB) fprintf(file, " c ");
        else if (value[4] == WGL_SWAP_UNDEFINED_ARB) fprintf(file, " . ");
        else fprintf(file, " . ");
        /* stereo */
        fprintf(file, " %c ", value[6] ? 'y' : '.');
        /* multisample */
        if (value[24] > 0)
            fprintf(file, "%2d | ", value[24]);
        else
            fprintf(file, " . | ");
        /* color size */
        if (value[8]) fprintf(file, "%3d ", value[8]);
        else fprintf(file, "  . ");
        /* red */
        if (value[9]) fprintf(file, "%2d ", value[9]); 
        else fprintf(file, " . ");
        /* green */
        if (value[10]) fprintf(file, "%2d ", value[10]); 
        else fprintf(file, " . ");
        /* blue */
        if (value[11]) fprintf(file, "%2d ", value[11]);
        else fprintf(file, " . ");
        /* alpha */
        if (value[12]) fprintf(file, "%2d | ", value[12]); 
        else fprintf(file, " . | ");
        /* aux buffers */
        if (value[20]) fprintf(file, "%2d ", value[20]);
        else fprintf(file, " . ");
        /* depth */
        if (value[18]) fprintf(file, "%2d ", value[18]);
        else fprintf(file, " . ");
        /* stencil */
        if (value[19]) fprintf(file, "%2d | ", value[19]);
        else fprintf(file, " . | ");
        /* accum size */
        if (value[13]) fprintf(file, "%3d ", value[13]);
        else fprintf(file, "  . ");
        /* accum red */
        if (value[14]) fprintf(file, "%2d ", value[14]);
        else fprintf(file, " . ");
        /* accum green */
        if (value[15]) fprintf(file, "%2d ", value[15]);
        else fprintf(file, " . ");
        /* accum blue */
        if (value[16]) fprintf(file, "%2d ", value[16]);
        else fprintf(file, " . ");
        /* accum alpha */
        if (value[17]) fprintf(file, "%2d | ", value[17]);
        else fprintf(file, " . | ");
        /* overlay */
        if (value[21]) fprintf(file, "%2d ", value[21]);
        else fprintf(file, " . ");
        /* underlay */
        if (value[22]) fprintf(file, "%2d ", value[22]);
        else fprintf(file, " . ");
        /* layer swap */
        if (value[23]) fprintf(file, "y ");
        else fprintf(file, " . ");
        fprintf(file, "|\n");
    }
    /* print table footer */
    fprintf(file, " +-----+-------------------------+-----------------+----------+-----------------+----------+\n");
    fprintf(file, " |     |          visual         |      color      | ax dp st |      accum      |   layer  |\n");
    fprintf(file, " |  id | tp ac gd fm db sw st ms |  sz  r  g  b  a | bf th cl |  sz  r  g  b  a | ov un sw |\n");
    fprintf(file, " +-----+-------------------------+-----------------+----------+-----------------+----------+\n");
}

static void VisualInfoGDI(HDC dc)
{
    int i, maxpf;
    PIXELFORMATDESCRIPTOR pfd;

    /* calling DescribePixelFormat() with NULL pfd (!!!) return maximum
    number of pixel formats */
    maxpf = DescribePixelFormat(dc, 1, 0, NULL);

    fprintf(file, "-----------------------------------------------------------------------------\n");
    fprintf(file, "   visual   x  bf  lv rg d st ge ge  r  g  b a  ax dp st   accum buffs    ms \n");
    fprintf(file, " id  dep tp sp sz  l  ci b ro ne ac sz sz sz sz bf th cl  sz  r  g  b  a ns b\n");
    fprintf(file, "-----------------------------------------------------------------------------\n");

    /* loop through all the pixel formats */
    for(i = 1; i <= maxpf; i++)
    {
        DescribePixelFormat(dc, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
        /* only describe this format if it supports OpenGL */
        if(!(pfd.dwFlags & PFD_SUPPORT_OPENGL)
            || ((pfd.dwFlags & PFD_DRAW_TO_BITMAP))) continue;
        /* other criteria could be tested here for actual pixel format
        choosing in an application:

        for (...each pixel format...) {
        if (pfd.dwFlags & PFD_SUPPORT_OPENGL &&
        pfd.dwFlags & PFD_DOUBLEBUFFER &&
        pfd.cDepthBits >= 24 &&
        pfd.cColorBits >= 24)
        {
        goto found;
        }
        }
        ... not found so exit ...
        found:
        ... found so use it ...
        */
        /* print out the information for this pixel format */
        fprintf(file, "0x%02x ", i);
        fprintf(file, "%3d ", pfd.cColorBits);
        if(pfd.dwFlags & PFD_DRAW_TO_WINDOW) fprintf(file, "wn ");
        else if(pfd.dwFlags & PFD_DRAW_TO_BITMAP) fprintf(file, "bm ");
        else fprintf(file, "pb ");
        /* should find transparent pixel from LAYERPLANEDESCRIPTOR */
        fprintf(file, " . "); 
        fprintf(file, "%3d ", pfd.cColorBits);
        /* bReserved field indicates number of over/underlays */
        if(pfd.bReserved) fprintf(file, " %d ", pfd.bReserved);
        else fprintf(file, " . "); 
        fprintf(file, " %c ", pfd.iPixelType == PFD_TYPE_RGBA ? 'r' : 'c');
        fprintf(file, "%c ", pfd.dwFlags & PFD_DOUBLEBUFFER ? 'y' : '.');
        fprintf(file, " %c ", pfd.dwFlags & PFD_STEREO ? 'y' : '.');
        /* added: */
        fprintf(file, " %c ", pfd.dwFlags & PFD_GENERIC_FORMAT ? 'y' : '.');
        fprintf(file, " %c ", pfd.dwFlags & PFD_GENERIC_ACCELERATED ? 'y' : '.');
        if(pfd.cRedBits && pfd.iPixelType == PFD_TYPE_RGBA) 
            fprintf(file, "%2d ", pfd.cRedBits);
        else fprintf(file, " . ");
        if(pfd.cGreenBits && pfd.iPixelType == PFD_TYPE_RGBA) 
            fprintf(file, "%2d ", pfd.cGreenBits);
        else fprintf(file, " . ");
        if(pfd.cBlueBits && pfd.iPixelType == PFD_TYPE_RGBA) 
            fprintf(file, "%2d ", pfd.cBlueBits);
        else fprintf(file, " . ");
        if(pfd.cAlphaBits && pfd.iPixelType == PFD_TYPE_RGBA) 
            fprintf(file, "%2d ", pfd.cAlphaBits);
        else fprintf(file, " . ");
        if(pfd.cAuxBuffers)     fprintf(file, "%2d ", pfd.cAuxBuffers);
        else fprintf(file, " . ");
        if(pfd.cDepthBits)      fprintf(file, "%2d ", pfd.cDepthBits);
        else fprintf(file, " . ");
        if(pfd.cStencilBits)    fprintf(file, "%2d ", pfd.cStencilBits);
        else fprintf(file, " . ");
        if(pfd.cAccumBits)   fprintf(file, "%3d ", pfd.cAccumBits);
        else fprintf(file, "  . ");
        if(pfd.cAccumRedBits)   fprintf(file, "%2d ", pfd.cAccumRedBits);
        else fprintf(file, " . ");
        if(pfd.cAccumGreenBits) fprintf(file, "%2d ", pfd.cAccumGreenBits);
        else fprintf(file, " . ");
        if(pfd.cAccumBlueBits)  fprintf(file, "%2d ", pfd.cAccumBlueBits);
        else fprintf(file, " . ");
        if(pfd.cAccumAlphaBits) fprintf(file, "%2d ", pfd.cAccumAlphaBits);
        else fprintf(file, " . ");
        /* no multisample in win32 */
        fprintf(file, " . .\n");
    }
    /* print table footer */
    fprintf(file, "-----------------------------------------------------------------------------\n");
    fprintf(file, "   visual   x  bf  lv rg d st ge ge  r  g  b a  ax dp st   accum buffs    ms \n");
    fprintf(file, " id  dep tp sp sz  l  ci b ro ne ac sz sz sz sz bf th cl  sz  r  g  b  a ns b\n");
    fprintf(file, "-----------------------------------------------------------------------------\n");
    fprintf(file, "\n");
    /* loop through all the pixel formats */
    for(i = 1; i <= maxpf; i++)
    {        
        DescribePixelFormat(dc, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
        /* only describe this format if it supports OpenGL */
        if(!(pfd.dwFlags & PFD_SUPPORT_OPENGL)
            || (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW))) continue;
        fprintf(file, "Visual ID: %2d  depth=%d  class=%s\n", i, pfd.cDepthBits, 
            pfd.cColorBits <= 8 ? "PseudoColor" : "TrueColor");
        fprintf(file, "    bufferSize=%d level=%d renderType=%s doubleBuffer=%ld stereo=%ld\n", pfd.cColorBits, pfd.bReserved, pfd.iPixelType == PFD_TYPE_RGBA ? "rgba" : "ci", pfd.dwFlags & PFD_DOUBLEBUFFER, pfd.dwFlags & PFD_STEREO);
        fprintf(file, "    generic=%d generic accelerated=%d\n", (pfd.dwFlags & PFD_GENERIC_FORMAT) == PFD_GENERIC_FORMAT, (pfd.dwFlags & PFD_GENERIC_ACCELERATED) == PFD_GENERIC_ACCELERATED);
        fprintf(file, "    rgba: redSize=%d greenSize=%d blueSize=%d alphaSize=%d\n", pfd.cRedBits, pfd.cGreenBits, pfd.cBlueBits, pfd.cAlphaBits);
        fprintf(file, "    auxBuffers=%d depthSize=%d stencilSize=%d\n", pfd.cAuxBuffers, pfd.cDepthBits, pfd.cStencilBits);
        fprintf(file, "    accum: redSize=%d greenSize=%d blueSize=%d alphaSize=%d\n", pfd.cAccumRedBits, pfd.cAccumGreenBits, pfd.cAccumBlueBits, pfd.cAccumAlphaBits);
        fprintf(file, "    multiSample=%d multisampleBuffers=%d\n", 0, 0);
        fprintf(file, "    Opaque.\n");
    }
}

static void VisualInfo(HDC dc)
{
    if (WGLEW_ARB_pixel_format)
        VisualInfoARB(dc);
    else
        VisualInfoGDI(dc);
}
