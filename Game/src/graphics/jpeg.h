#ifndef _JPEG_H
#define _JPEG_H

typedef struct TexReadInfo TexReadInfo;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Decode size bytes of JPEG data from mem into packed RGB pixels in info.
 * Returns 1 on success, transferring a buffer released with free() to info.
 * Returns 0 for invalid input or decoding failure, leaving info unchanged.
 * mem and info may be NULL, which fails; the input memory remains borrowed.
 * Decoder state is local to the call; concurrent calls need separate outputs.
 */
int jpegLoad(char *mem, int size, TexReadInfo *info);

void jpgSave( char * name, U8 * pixbuf, int bpp, int x, int y );
void jpgSaveEx( char * name, U8 * pixbuf, int bpp, int x, int y, char *extraJpegData, int extraJpegDataLen);
#ifdef __cplusplus
 }
#endif

#endif
