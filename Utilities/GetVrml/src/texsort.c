#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "texsort.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include "texloaddesc.h"

TexName    tex_names[1000];
int        tex_name_count;

void texNameClear(int addwhite)
{
    tex_name_count = 0;
    if (addwhite)
        texNameAdd("white.tga"); // Default texture value
}

void texLoadAll()
{
    texLoadAllInfo(1);
    texNameClear(1);
}

int texNameAdd(char *name)
{
    int        i;
    char    *s;
    // get rid of pathname to texture (usually max adds something like ../maps/)
    s = strrchr(name,'/');
    if (!s)
        s = strrchr(name,'\\');
    if (s)
        name = s + 1;
    for(i=0;i<tex_name_count;i++)
    {
        if (strcmp(tex_names[i].name,name)==0)
            return i;
    }
    strcpy(tex_names[i].name,name);
    tex_names[i].ti = getTexInf(name);
    tex_name_count++;
    return tex_name_count-1;
}

int cmpTex(const void* aData, const void* bData)
{
    int * a = (int *)aData;
    int * b = (int *)bData;
int        aa=0,ba=0;

    if (tex_names[*a].ti)
        aa = tex_names[*a].ti->tfh.alpha;
    if (tex_names[*b].ti)
        ba = tex_names[*b].ti->tfh.alpha;
    return aa-ba;
}

void reorderTriIdxsByTex(GMesh *mesh)
{
    gmeshSortTrisByTexID(mesh, cmpTex);
}

void texPrintUsage()
{
int        i;
FILE    *file;
char    buf[1000];

    file = fileOpen("getvrml_texusage.txt","wt");
    for(i=0;i<tex_inf_count;i++)
    {
        strcpy(buf,tex_infs[i].name);
        if (buf[strlen(buf)-1] == '\r')
            buf[strlen(buf)-1] = 0;
        fprintf(file,"%s\n",buf);
    }
    fclose(file);
}
