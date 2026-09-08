#include "graphics/FX/fxfluid.h"
#include <string.h>
#include "graphics/FX/fx.h"
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include "cmdparse/cmdgame.h"
#include "graphics/font.h"
#include "time.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/error.h>
#include "seq/tricks.h"
#include "render/tex.h"
#include <utilitieslib/utils/textparser.h>


#include <utilitieslib/utils/strings_opt.h>

#if NOVODEX

extern FxEngine fx_engine;

// global lists of parsed objects
typedef struct FxFluidList {
    FxFluid **fluids;
} FxFluidList;

FxFluidList fx_fluidlist;

TokenizerParseInfo ParseFxFluid[] = {
    { "Fluid",                    TOK_IGNORE,                    0                            }, // hack, so we can use the old list system for a bit
    { "Name",                    TOK_CURRENTFILE(FxFluid,name)                            },
    { "FileAge",                TOK_TIMESTAMP(FxFluid,fileAge)                            },
    { "maxParticles",            TOK_INT(FxFluid,maxParticles,                100)        },
    { "restParticlesPerMeter",    TOK_F32(FxFluid,restParticlesPerMeter,        50.0f)        },
    { "restDensity",            TOK_F32(FxFluid,restDensity,                1000.0f)    },
    { "stiffness",                TOK_F32(FxFluid,stiffness,                    20.0f)        },
    { "viscosity",                TOK_F32(FxFluid,viscosity,                    6.0f)        },
    { "damping",                TOK_F32(FxFluid,damping,                    0)            }, // 0.1f) },
    { "restitution",            TOK_F32(FxFluid,restitution,                0)            }, // 0.2f) },
    { "adhesion",                TOK_F32(FxFluid,adhesion,                    0)            }, // 0.1f) },
    { "dimensionX",                TOK_F32(FxFluid,dimensionX,                    1.0f)        },
    { "dimensionY",                TOK_F32(FxFluid,dimensionY,                    1.0f)        },
    { "rate",                    TOK_F32(FxFluid,rate,                        20.0f)        },
    { "randomAngle",            TOK_F32(FxFluid,randomAngle,                0)            }, // 0.1f) },
    { "fluidVelocityMagnitude",    TOK_F32(FxFluid,fluidVelocityMagnitude,        0.0f)        },
    { "particleLifetime",        TOK_F32(FxFluid,particleLifetime,            0.0f)        },

    { "TextureName",            TOK_FIXEDSTR(FxFluid,parttex[0].texturename)            },
    { "TextureName2",            TOK_FIXEDSTR(FxFluid,parttex[1].texturename)            },
    { "TexScroll1",              TOK_VEC3(FxFluid,parttex[0].texscroll)                    },
    { "TexScroll2",              TOK_VEC3(FxFluid,parttex[1].texscroll)                    },

    { "TexScrollJitter1",        TOK_VEC3(FxFluid,parttex[0].texscrolljitter)            },
    { "TexScrollJitter2",        TOK_VEC3(FxFluid,parttex[1].texscrolljitter)            },
    { "AnimFrames1",            TOK_F32(FxFluid,parttex[0].animframes,0)                },
    { "AnimFrames2",            TOK_F32(FxFluid,parttex[1].animframes,0)                },

    { "AnimPace1",                TOK_F32(FxFluid,parttex[0].animpace,0)                    },
    { "AnimPace2",                TOK_F32(FxFluid,parttex[1].animpace,0)                    },
    { "AnimType1",                TOK_INT(FxFluid,parttex[0].animtype,0)                    },
    { "AnimType2",                TOK_INT(FxFluid,parttex[1].animtype,0)                    },
    { "startSize",                TOK_F32(FxFluid,startSize,                    0)            }, // 0.3f) },
    { "alpha",                    TOK_INT(FxFluid,alpha,                        255)        },

    { "StartColor",                TOK_RGB(FxFluid,colornavpoint[0].rgb)                    },
    { "BeColor1",                TOK_RGB(FxFluid,colornavpoint[1].rgb)                    },
    { "BeColor2",                TOK_RGB(FxFluid,colornavpoint[2].rgb)                    },
    { "BeColor3",                TOK_RGB(FxFluid,colornavpoint[3].rgb)                    },
    { "BeColor4",                TOK_RGB(FxFluid,colornavpoint[4].rgb)                    },

    { "ByTime1",                TOK_INT(FxFluid,colornavpoint[1].time,0)                },
    { "ByTime2",                TOK_INT(FxFluid,colornavpoint[2].time,0)                },
    { "ByTime3",                TOK_INT(FxFluid,colornavpoint[3].time,0)                },
    { "ByTime4",                TOK_INT(FxFluid,colornavpoint[4].time,0)                },

    { "End",                    TOK_END                                                    },
    { "",                        0, 0                                                    }
};

TokenizerParseInfo ParseFxFluidList[] = {
    { "Fluid",                TOK_STRUCT(FxFluidList,fluids, ParseFxFluid) },
    { "", 0, 0 }
};




static FxFluid * fxLoadFxFluid( char fname[] )
{
    FxFluid * fxfluid = 0;
    int         fileisgood = 0;
    TokenizerHandle tok;
    char buf[1000];


    errorLogFileIsBeingReloaded(fname);

    tok = TokenizerCreate(fname);
    if (tok) 
    {
        fxfluid = (FxFluid*)listAddNewMember(&fx_engine.fxfluids, sizeof(FxFluid));
        assert(fxfluid);
        listScrubMember(fxfluid, sizeof(FxFluid));
        fileisgood = TokenizerParseList(tok, ParseFxFluid, fxfluid, TokenizerErrorCallback);
        if( !fileisgood )
        {
            listFreeMember( fxfluid, &fx_engine.fxfluids );
            fxfluid = 0;
        }
        else
        {
            fxCleanFileName(buf, fxfluid->name); //(buf2 will always be shorter than info->name)
            strcpy(fxfluid->name, buf);
            fx_engine.fxfluidcount++;
        }
    }
    TokenizerDestroy(tok);




    return fxfluid;
}


static int fxFluidNameCmp(const void* info1Data, const void* info2Data)
{
    const FxFluid ** info1 = (const FxFluid **)info1Data;
    const FxFluid ** info2 = (const FxFluid **)info2Data;
    return stricmp( (*info1)->name, (*info2)->name );
}

static int fxFluidNameCmp2(const void* info1Data, const void* info2Data)
{
    const FxFluid * info1 = (const FxFluid *)info1Data;
    const FxFluid ** info2 = (const FxFluid **)info2Data;
    return stricmp( info1->name, (*info2)->name );
}

void initFluid( FxFluid* fxfluid )
{    
    int i;

    // Set texture animation parameters
    for(i = 0 ; i < 2 ; i++ )
    {
        fxfluid->parttex[i].bind = texLoadBasic(fxfluid->parttex[i].texturename,TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_FX);

        assert(fxfluid->parttex[i].bind);

        if(fxfluid->parttex[i].animframes <= 1.0) 
        {
            fxfluid->parttex[i].framewidth    = 1;
            fxfluid->parttex[i].texscale[0] = 1.0;
            fxfluid->parttex[i].texscale[1] = 1.0;
        }
        else if(fxfluid->parttex[i].animframes <= 2.0) 
        {
            fxfluid->parttex[i].framewidth    = 2;
            fxfluid->parttex[i].texscale[0] = 0.5;
            fxfluid->parttex[i].texscale[1] = 1.0;
        }
        else if(fxfluid->parttex[i].animframes <= 4.0) 
        {
            fxfluid->parttex[i].framewidth    = 2;
            fxfluid->parttex[i].texscale[0] = 0.5;
            fxfluid->parttex[i].texscale[1] = 0.5;
        }
        else if(fxfluid->parttex[i].animframes <= 8.0) 
        {
            fxfluid->parttex[i].framewidth    = 4;
            fxfluid->parttex[i].texscale[0] = 0.25;
            fxfluid->parttex[i].texscale[1] = 0.5;
        }
        else if(fxfluid->parttex[i].animframes <= 16.0) 
        {
            fxfluid->parttex[i].framewidth    = 4;
            fxfluid->parttex[i].texscale[0] = 0.25;
            fxfluid->parttex[i].texscale[1] = 0.25;
        }
        else if(fxfluid->parttex[i].animframes <= 32.0) 
        {
            fxfluid->parttex[i].framewidth    = 8;
            fxfluid->parttex[i].texscale[0] = 0.125;
            fxfluid->parttex[i].texscale[1] = 0.25;
        }
        else 
            assert(0);
    }
    // If either of these asserts go off, the optimizing compiler appears to have messed up.
    assert(fxfluid->parttex[0].bind);
    assert(fxfluid->parttex[1].bind);

    //TO DO fix this so it does a better job
    if(    !fxfluid->colornavpoint[0].rgb[0] &&
        !fxfluid->colornavpoint[0].rgb[1] && 
        !fxfluid->colornavpoint[0].rgb[2])
    {
        fxfluid->colornavpoint[0].rgb[0] = 255;
        fxfluid->colornavpoint[0].rgb[1] = 255;
        fxfluid->colornavpoint[0].rgb[2] = 255;
    }

    partBuildColorPath(&fxfluid->colorPathDefault, fxfluid->colornavpoint, 0, false);
    fxfluid->colornavpoint[0].time = 0; //mm temp

}


FxFluid * fxGetFxFluid( char fluid_name[] )
{
    FxFluid * fxfluid = 0;
    char fx_name_cleaned_up[FX_MAX_PATH];
    int numfluids;
    FxFluid dummy = {0};
    FxFluid * * dptr;

    fxCleanFileName(fx_name_cleaned_up, fluid_name);

    // ## Search Mark's binary of loaded fxfluids. 
    dummy.name = fx_name_cleaned_up;
    numfluids = eaSize(&fx_fluidlist.fluids);
    dptr = bsearch(&dummy, fx_fluidlist.fluids, numfluids,
        sizeof(FxFluid*),fxFluidNameCmp2);
    if( dptr )
    {
        fxfluid = *dptr;
        assert(fxfluid);
        //Development only
        if (!global_state.no_file_change_check && isDevelopmentMode())
            fxfluid->fileAge = fileLastChanged( "bin/fluids.bin" );
        //End developmetn only
    }

    //Failure in production mode == very bad data.

    // ## Development only 
    if (!global_state.no_file_change_check && isDevelopmentMode()) 
    {
        // always prefer the debug version to the binary version 
        {
            FxFluid * dev_fxfluid;
            for( dev_fxfluid = fx_engine.fxfluids ; dev_fxfluid ; dev_fxfluid = dev_fxfluid->next )
            {
                if( !_stricmp(fx_name_cleaned_up, dev_fxfluid->name ) )
                {
                    fxfluid = dev_fxfluid;
                    break;    
                }
            }
        }

        {
            char buf[500];
            sprintf(buf, "%s%s", "fx/", fx_name_cleaned_up);

            //Reload the sysinfo from the file under these conditions
            if( !fxfluid || //it's brand new fxfluid
                (fxfluid && fileHasBeenUpdated( buf, &fxfluid->fileAge )) )  //the fxfluid file itself has changed
            {
                fxfluid = fxLoadFxFluid(buf);
            }
        }
    }
    // ## End development only

    if( !fxfluid )
    {
        printToScreenLog( 1, "INFO: Failed to get fluid %s", fx_name_cleaned_up ); 
    }
    else
    {
        if ( !fxfluid->initialized )
        {
            initFluid(fxfluid);
            fxfluid->initialized = 1;
        }
    }

    // ## Developnment only: if fxfluid fails for any reason, try to remove it from the run time load list
    if( !fxfluid )
    {
        FxFluid * checkerfluid;
        FxFluid * checkerfluid_next;
        for( checkerfluid = fx_engine.fxfluids ; checkerfluid ; checkerfluid = checkerfluid_next )
        {
            checkerfluid_next = checkerfluid->next;
            if( !checkerfluid->name || !_stricmp( fx_name_cleaned_up, checkerfluid->name ) )
            {
                listFreeMember( checkerfluid, &fx_engine.fxfluids );
                fx_engine.fxfluidcount--;
                assert( fx_engine.fxfluidcount >= 0 );
            }
        }
    }
    //## end development only

    return fxfluid;
}

void fxPreloadFluidInfo()
{
    char *dir=game_state.nofx?NULL:"fx";
    char *filetype=game_state.nofx?"fx/fluids/null.fluid":".fluid";
    int flags=0;
    char *binFilename = game_state.nofx?0:"fluids.bin";
    ParserLoadFiles(dir, filetype, binFilename, flags, ParseFxFluidList, &fx_fluidlist, NULL, NULL, NULL);

    {
        char buf2[FX_MAX_PATH];
        int num_structs, i;
        FxFluid * fluid;

        //Clean Up Fluid names, and sort them
        num_structs = eaSize(&fx_fluidlist.fluids);
        for (i = 0; i < num_structs; i++)
        {
            fluid = fx_fluidlist.fluids[i];
            fxCleanFileName(buf2, fluid->name); //(buf2 will always be shorter than info->name)
            strcpy(fluid->name, buf2);
        }
        qsort(fx_fluidlist.fluids, num_structs, sizeof(void*), fxFluidNameCmp);
    }
}

#endif