
#include <utilitieslib/utils/utils.h>  // for strstri
#include <utilitieslib/components/Earray.h>
#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "entity/EntPlayer.h"
#include "entity/entclient.h"  // for entSetAlpha
#include "entity/costume_client.h"

#include "gameData/costume_data.h"
#include "language/langClientUtil.h"

#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiAvatar.h"
#include "UI/uiCostume.h"
#include "UI/uiTailor.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiSupercostume.h"
#include "UI/uiSuperRegistration.h"
#include "UI/uiEditText.h"
#include "UI/uiNet.h"
#include "UI/uiScrollBar.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include "render/tex.h"
#include "UI/uiBox.h"
#include "sound/sound.h"
#include "graphics/ttFontUtil.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/character_base.h"
#include "clientcomm/clientcomm.h"

#define RING_WD     30              // Width for a single color ring
#define PICKER_X    72              // All picker rows start at this x
#define PICKER_Z    10
#define PICKER_HT   30              // How tall is a single picker row?
#define PICKER_WD   615             // How wide is a single picker row?
#define PICKER_VERT_SPACING 10      // How far apart should the picker rows be?

#define PICKER_C1_X 292             // abs x coordinates where the Primary color column starts
#define PICKER_C2_X 515             // abs x coordinates where the Secondary color column starts.
#define SUPER_COLOR1_OFFSET 64      // x coordinates of the first supergroup color ring relative to beginning of column.
#define SUPER_COLOR2_OFFSET 98      // x coordinates of the 2nd   supergroup color ring relative to beginning of column.
#define SUPER_COLOR_SPACING 4
#include "entity/Supergroup.h"

static char gSuperEmblemName[256];

Costume * gSuperCostume;
// sets the supergroup emblem
void supercostume_SetEmblem( char * name )
{
    strcpy( gSuperEmblemName, name );
}

//---------------------------------------------------------------------------------------
// Entry/Exit Functions /////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------

// copy original costume into supercostume set, set it as current
//
static void supercostume_init()
{
    Entity *e = playerPtr();
    e->costume = costume_as_const(gSuperCostume);
    entSetAlpha( e, 255, SET_BY_CAMERA );
}

void costume_ZeroStruct( CostumeOriginSet *cset )
{
    int i, j, k;

    for( i = 0; i < eaSize(&cset->regionSet); i++ )
    {
        CostumeRegionSet *rset = cpp_const_cast(CostumeRegionSet*)(cset->regionSet[i]);
        rset->currentBoneSet = 0;
        for( j = 0; j < eaSize( &rset->boneSet); j++ )
        {
            CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(rset->boneSet[j]);
            bset->currentSet = 0;

            for( k = 0; k < eaSize(&bset->geo); k++ )
            {
                CostumeGeoSet *gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[k]);
                gset->currentInfo = 0;
                gset->currentMask = 0;
            }
        }
    }
}

// figure out everything we need to know to get the costume...hairy
//
void costume_MatchPart( Entity *e, const CostumePart *cpart, const CostumeOriginSet *cset, int supergroup, const char * partName )
{
    int i, j, k, m, n;

    if (!partName)
        return;

     for( i = 0; i < eaSize(&cset->regionSet); i++ )
    {
        CostumeRegionSet *rset = cpp_const_cast(CostumeRegionSet*)(cset->regionSet[i]);
        if( cpart->regionName && stricmp( rset->name, cpart->regionName ) == 0 ) // match region
        {
            for( j = 0; j < eaSize( &rset->boneSet); j++ )
            {
                CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(rset->boneSet[j]);

                //if( bonesetIsRestricted(bset) )
                //    continue;

                if( !cpart->bodySetName || bonesetIsRestricted(bset) )
                {
                    rset->currentBoneSet = 0; // update costume structure so we know where we are
                    bset->currentSet = 0;
                    continue;
                }

                if( stricmp( bset->name, cpart->bodySetName ) == 0 || // match bone set
                    (stricmp( bset->name, "weapons") == 0 && stricmp( cpart->bodySetName, "arachnos weapons") == 0)) //HACK Previously we had a seperate body part name for ararchnos
                {
                    for( k = 0; k < eaSize(&bset->geo); k++ )
                    {
                        CostumeGeoSet *gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[k]);

                        if( gset->faces )// || geosetIsRestricted(gset) )
                            continue;

                        if( stricmp( partName, gset->bodyPartName ) == 0 ) // match geoset
                        {
                            for( m = 0; m < eaSize(&gset->info); m++ )
                            {
                                // set defaults just in case nothing is found
                                 bset->currentSet = 0;
                                gset->currentInfo = 0;

                                if( partIsRestricted(gset->info[m]) )
                                    continue;

                                if( stricmp( gset->info[m]->texName1, cpart->pchTex1 ) == 0 && // match GeoTex Set
                                    stricmp( gset->info[m]->geoName, cpart->pchGeom ) == 0 && // Chestdetail is special case where unique key is in tex2 instead of tex1...
                                    (isNullOrNone(cpart->pchFxName) || ( gset->info[m]->fxName && stricmp( gset->info[m]->fxName, cpart->pchFxName ) == 0 )) &&
                                    ( stricmp( gset->info[m]->texName2, cpart->pchTex2 ) == 0 || gset->info[m]->texMasked || stricmp( gset->info[m]->texName2, "mask" ) == 0 )
                                  )
                                {
                                    int foundmask = 1;

                                    //    hack version 3
                                    //    if the piece is texMasked, see if we can find a better match
                                    //    if not, use the piece, but if there exists a better match, use it
                                    if (gset->info[m]->texMasked || (stricmp(gset->info[m]->texName2, "mask") == 0))
                                    {
                                        int n;
                                        for (n = m+1; n < eaSize(&gset->info); n++)
                                        {
                                            if( stricmp( gset->info[n]->texName1, cpart->pchTex1 ) == 0 && // match GeoTex Set
                                                stricmp( gset->info[n]->geoName, cpart->pchGeom ) == 0 && // Chestdetail is special case where unique key is in tex2 instead of tex1...
                                                (isNullOrNone(cpart->pchFxName) || ( gset->info[n]->fxName && stricmp( gset->info[n]->fxName, cpart->pchFxName ) == 0 )) &&
                                                ( stricmp( gset->info[n]->texName2, cpart->pchTex2 ) == 0 )
                                                )
                                            {
                                                m = n;    //    if we can find a better match, use that match instead
                                                break;
                                            }
                                        }
                                    }

                                    if( stricmp( gset->info[m]->texName2, "mask" ) == 0 )
                                        foundmask = 0;

                                    // found a match
                                     rset->currentBoneSet = j; // update costume structure so we know where we are
                                    bset->currentSet = k;
                                    gset->currentInfo = m;

                                    //now get the texture
                                    if( gset->info[m]->texMasked )
                                    {
                                        if( stricmp( gset->info[m]->texName2, cpart->pchTex2 ) == 0 )
                                        {
                                            gset->currentMask = -1;
                                            foundmask = 1;
                                        }
                                    }

                                    if ( gset->mask ) // check the nefangled mask array first
                                    {
                                        for( n = 0; n < eaSize(&gset->mask); n++ )
                                        {
                                            if( maskIsRestricted( gset->mask[n] ) )
                                                continue;

                                            if( stricmp( gset->mask[n]->name, cpart->pchTex2 ) == 0 )
                                            {
                                                gset->currentMask = n;
                                                foundmask = 1;
                                            }
                                        }
                                    }
                                    else // ok try the old one now
                                    {
                                        for( n = 0; n < eaSize(&gset->masks); n++ )
                                        {
                                            if( stricmp( gset->masks[n], cpart->pchTex2 ) == 0 )
                                            {
                                                gset->currentMask = n;
                                                foundmask = 1;
                                                break;
                                            }
                                        }
                                    }

                                    if( foundmask )
                                        return;
                                } // end if found texset
                            } // end for every texset
                        }// end if found geoset
                    } // end for every geoset
                } // end if found boneset
            } // end for every bone set
        }// end if found region set
    } // end for every region set
}


// Back tracks from the stored data to re-create costume in terms of character creation data
//
void costume_setCurrentCostume( Entity *e , int supergroup )
{
    // first the body set and originset
    //----------------------------------------------------------------------------------------------
    char * genderName[] = { "Male", "Female", "BasicMale", "BasicFemale", "Huge", "Villain" };
    CostumeOriginSet *cset = 0;
    int i, j;

    // set the bodyType index and origin index
    for( i = 0; i < eaSize( &gCostumeMaster.bodySet ); i++ )
    {
        if ( stricmp( gCostumeMaster.bodySet[i]->name, genderName[e->pl->costume[e->pl->current_costume]->appearance.bodytype] ) == 0 )
        {
            gCurrentBodyType = i;

            for( j = 0; j < eaSize( &gCostumeMaster.bodySet[i]->originSet ); j++ )
            {
                if ( costume_checkOriginForClassAndSlot( gCostumeMaster.bodySet[i]->originSet[j]->name, e->pchar->pclass, e->pl->current_costume ) )
                {
                    gCurrentOriginSet = j;
                    gCurrentRegion = REGION_SETS;
                    break;
                }
            }
            break;
        }
    }

    // now go flesh out all of the parts
    cset = cpp_const_cast(CostumeOriginSet*)(costume_get_current_origin_set());
    if( supergroup )
        costume_ZeroStruct(cset);

    for( i = 0; i < e->pl->costume[0]->appearance.iNumParts; i++ )
    {
        const CostumePart* part = e->costume->parts[i];
        CostumePart* mutable_part = cpp_const_cast(CostumePart*)(part);
        if ( part->pchGeom && stricmp( part->pchGeom, "none" ) == 0 &&
             isNullOrNone(part->pchFxName) )
        {
            if (stricmp( bodyPartList.bodyParts[i]->name, "ChestDetail" ) == 0 )
            {
                mutable_part->colorTmp[0] = CreateColorRGB(0, 0, 0);
                mutable_part->colorTmp[1] = CreateColorRGB(255, 255, 255);
            }
            else
            {
                costume_MatchPart( e, mutable_part, cset, supergroup, bodyPartList.bodyParts[i]->name );
                continue;
            }
        }

        costume_MatchPart( e, mutable_part, cset, supergroup, bodyPartList.bodyParts[i]->name );
        for (j=0; j<4; j++) 
            mutable_part->colorTmp[j] = e->pl->costume[e->pl->current_costume]->parts[i]->color[j];
    }
}

// Save the supergroup costume, switch back to the original costume
//
static void supercostume_Finalize()
{
    Entity *e = playerPtr();
    SGColorBits prim, sec, prim2, sec2, tert, quat;

    costume_SGColorsExtract(e, gSuperCostume, &prim, &sec, &prim2, &sec2, &tert, &quat);
    sendSuperCostume( prim, sec, prim2, sec2, tert, quat, e->pl->hide_supergroup_emblem );
    costume_destroy( gSuperCostume );
    gSuperCostume = 0;
}

//---------------------------------------------------------------------------------------
// Drawing Functions ////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------

Color getColor( Vec3 clr )
{
    int r = clr[0];
    int g = clr[1];
    int b = clr[2];

    return CreateColorRGB( r, g, b );
}

typedef struct CostumeWeaponStancePair
{
    const char *weapon;
    int tailorStance;
}CostumeWeaponStancePair;

typedef struct CostumeWeaponStancePairList
{
    const CostumeWeaponStancePair **weaponStanceList;
}CostumeWeaponStancePairList;


CostumeWeaponStancePairList g_CostumeWeaponStancePairList;
StaticDefineInt ParseCostumeWeaponStanceType[] = {
    DEFINE_INT
    { "None",                0 },
    { "LeftHand",            TAILOR_STANCE_LEFTHAND },
    { "RightHand",            TAILOR_STANCE_RIGHTHAND },
    { "DualHand",            TAILOR_STANCE_DUALHAND },
    { "Katana",                TAILOR_STANCE_LEFTHAND | TAILOR_STANCE_KATANA },
    { "Rifle",                TAILOR_STANCE_TWOHAND | TAILOR_STANCE_GUN },
    { "EpicRight",            TAILOR_STANCE_EPICRIGHTHAND    },
    { "DualHandLarge",        TAILOR_STANCE_TWOHAND_LARGE },
    { "Shield",                TAILOR_STANCE_SHIELD},
    DEFINE_END
};
TokenizerParseInfo ParseWeaponStancePair[]=
{
    { "",   TOK_STRUCTPARAM|TOK_STRING(CostumeWeaponStancePair, weapon, 0 ) },
    { "",   TOK_STRUCTPARAM|TOK_INT(CostumeWeaponStancePair, tailorStance, 0),ParseCostumeWeaponStanceType },
    { "\n",   TOK_STRUCTPARAM|TOK_END,  0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCostumeWeaponStanceList[]=
{
    { "Weapon",   TOK_STRUCT(CostumeWeaponStancePairList, weaponStanceList,  ParseWeaponStancePair),  0 },
    { "", 0, 0 }
};
void load_CostumeWeaponStancePairList()
{
    ParserLoadFiles(NULL,"defs/costumeWeaponStances.def","costumeWeaponStances.bin",0,
        ParseCostumeWeaponStanceList, 
        &g_CostumeWeaponStancePairList,
        NULL,NULL,NULL);
}

/* To be added as well ...
* Katana scrap
* Claws scrap
* BS scrap
* DB scrap
- BS / shield scrap
* martial arts / shield scrap
* TA defender
- arch blaster
* AR blaster
- mace tank
* axe tank
- mace/shield tank
- mace/shield brute
- axe/shield tank
stone tank
- stone/shield tank
* fire shield tank

* thugs / TA MM
*/


void costume_SetStanceFor(const CostumePart *cpart, int *stanceBits)
{
    unsigned int pString;
    
    *stanceBits = TAILOR_STANCE_PROFILE | TAILOR_STANCE_HIGH;
    if (cpart == NULL || cpart->displayName == NULL)
    {
        return;
    }
    if (cpart->pchFxName)
    {
        char tempStr[MAX_PATH];
        char *result;
        strcpy_s(tempStr, MAX_PATH,cpart->pchFxName);
        result = strtok(tempStr, "/\\");
        if (result)
        {
            result = strtok(NULL, "/\\");
            if (result)
            {
                int i;
                for (i = 0; i < eaSize(&g_CostumeWeaponStancePairList.weaponStanceList); ++i)
                {
                    if (stricmp(g_CostumeWeaponStancePairList.weaponStanceList[i]->weapon, result) == 0)
                    {
                        *stanceBits = TAILOR_STANCE_PROFILE | TAILOR_STANCE_WEAPON | TAILOR_STANCE_COMBAT | g_CostumeWeaponStancePairList.weaponStanceList[i]->tailorStance;
                        return;
                    }
                }
            }
        }
    }

    if (sscanf(cpart->displayName, "P%u", &pString) != 1)
    {
        return;
    }
    // This is an even worse hack.  We only want the aura to enter combat mode
    // if it's one of the combat auras, so we specifically look for those here.
    if (pString == 3636920680) // Effect
    {
        // We can't use the table above since that would drop into combat stance
        // for all auras, so we check and see if this is a combat aura
        if (stricmp(cpart->bodySetName, "Combat Aura") == 0)
        {
            *stanceBits = TAILOR_STANCE_PROFILE | TAILOR_STANCE_COMBAT;
        }
    }
}

//
static void supercostume_ColorPick( int x, int y, int z, CostumePart *cpart, int idx, int color_num, float screenScaleX, float screenScaleY )
{
    Entity *e = playerPtr();
    CRDMode defaultMode = CRD_MOUSEOVER | CRD_SELECTABLE;
    CRDMode mode = defaultMode;
    Color color, colorTmp;
    int colorindex=color_num-1;
    float screenScale = MIN(screenScaleX, screenScaleY);
     if( color_num == 1 )
    {
        x += RING_WD/2;
        color.integer     = cpart->color[0].integer; 
        colorTmp.integer = cpart->colorTmp[0].integer; 
    }
    else if( color_num == 2 )
    {
        color.integer     = cpart->color[1].integer; 
        colorTmp.integer = cpart->colorTmp[1].integer; 
    }
    else if( color_num == 3 )
    {
        x += RING_WD/2;
        color.integer     = cpart->color[2].integer; 
        colorTmp.integer = cpart->colorTmp[2].integer; 
    }
    else if( color_num == 4 )
    {
        color.integer     = cpart->color[3].integer; 
        colorTmp.integer = cpart->colorTmp[3].integer; 
    }
     color.integer |= 0xff000000;
    colorTmp.integer |= 0xff000000;

    // Original Color
     if( color.integer == colorTmp.integer )
        mode |= CRD_SELECTED;

    if( colorRingDraw( x*screenScaleX, y*screenScaleY, z, mode, colorFlip(colorTmp).integer, screenScale ) )
    {
        costume_PartSetColor( costume_as_mutable_cast(e->costume), idx, colorindex, colorTmp );
        costume_SetStanceFor(cpart, &gTailorStanceBits);
    }

    // Super Color 1
    mode = defaultMode;
    if( colorFlip(color).integer == e->supergroup->colors[0] )
        mode |= CRD_SELECTED;

    if( colorRingDraw( (x+SUPER_COLOR1_OFFSET)*screenScaleX, y*screenScaleY, z, mode, e->supergroup->colors[0], screenScale ) )
    {
        costume_PartSetColor( costume_as_mutable_cast(e->costume), idx, colorindex, colorFlip(COLOR_CAST(e->supergroup->colors[0])));
        costume_SetStanceFor(cpart, &gTailorStanceBits);
    }

    // Super Color 2
    if(e->supergroup->colors[0] != e->supergroup->colors[1])
    {
        mode = defaultMode;
        if( colorFlip(color).integer == e->supergroup->colors[1] )
            mode |= CRD_SELECTED;

        if( colorRingDraw( (x+SUPER_COLOR2_OFFSET)*screenScaleX, y*screenScaleY, z, mode, e->supergroup->colors[1], screenScale ) )
        {
            costume_PartSetColor( costume_as_mutable_cast(e->costume), idx, colorindex, colorFlip(COLOR_CAST(e->supergroup->colors[1])));
            costume_SetStanceFor(cpart, &gTailorStanceBits);
        }
    }
}

//
//
static int supercostume_DrawPartPicker( int y, CostumePart * cpart, int idx, int colors34, float screenScaleX, float screenScaleY )
{
    float screenScale = MIN(screenScaleX, screenScaleY);
    if( stricmp( cpart->pchGeom, "none" ) == 0 && isNullOrNone(cpart->pchFxName) ) // be sure not to clear costume data because of empty parts
        return 0;

       drawFrame( PIX3, R10, PICKER_X*screenScaleX, y*screenScaleY, PICKER_Z, PICKER_WD*screenScaleX, PICKER_HT*screenScaleY, 1.f, SELECT_FROM_UISKIN( 0x24406f88, 0xffffff88, 0x34346f88 ), SELECT_FROM_UISKIN( 0x004dab88, 0x88000088, 0x22228888 ) );

     font( &game_12 );
    if( game_state.skin != UISKIN_HEROES )
         font_color( CLR_WHITE, CLR_WHITE );
    else
        font_color( CLR_ONLINE_ITEM, CLR_ONLINE_ITEM );

    if( colors34 )
        prnt( (PICKER_X + 28)*screenScaleX,( y + PICKER_HT - 10)*screenScaleY, PICKER_Z+1, screenScale, screenScale, "CapePartInside", cpart->displayName, "InsideParen" );
    else
        prnt( (PICKER_X + 28)*screenScaleX,( y + PICKER_HT - 10)*screenScaleY, PICKER_Z+1, screenScale, screenScale, (char*)cpart->displayName );


    supercostume_ColorPick( PICKER_C1_X, y + PICKER_HT/2, PICKER_Z+5, cpart, idx, colors34?3:1, screenScaleX, screenScaleY );

    {
        AtlasTex *dot = atlasLoadTexture( "EnhncTray_powertitle_dot.tga" );
        float dotHeight = dot->height;
        reverseToScreenScalingFactorf(NULL, &dotHeight);
        display_sprite( dot, (85-3)*screenScaleX, (y + (PICKER_HT-dotHeight)/2)*screenScaleY, PICKER_Z+1, screenScale, screenScale, CLR_WHITE );
    }

    if( !strstriConst( cpart->pchTex1, "skin" ) )
        supercostume_ColorPick( PICKER_C2_X+RING_WD/2, y + PICKER_HT/2, PICKER_Z+5, cpart, idx, colors34?4:2, screenScaleX, screenScaleY );

    return 1;
}


//
//
static int supercostume_drawAllPickers(Entity * e, UIBox box, float screenScaleX, float screenScaleY)
{
    int i, y = box.y + 48;

    for( i = 0; i < e->costume->appearance.iNumParts; i++ )
    {
        const CostumePart* part = e->costume->parts[i];
        CostumePart* mutable_part = cpp_const_cast(CostumePart*)(part);
        if( supercostume_DrawPartPicker( y, mutable_part, i, 0, screenScaleX, screenScaleY ) )
            y += PICKER_HT + PICKER_VERT_SPACING;

        if( !isNullOrNone(part->pchFxName) &&
            ( (stricmp(part->regionName, "capes") == 0 && stricmp(part->bodySetName, "wings") != 0) ||
              (stricmp(part->bodySetName, "trench coat") == 0) ) )
        {
            if( supercostume_DrawPartPicker( y, mutable_part, i, 1, screenScaleX, screenScaleY ) )
                y += PICKER_HT + PICKER_VERT_SPACING;
        }
    }

    costume_Apply(e);

       return y - (box.y)+PICKER_HT + PICKER_VERT_SPACING;
}

static void drawColumnColorRingLabels(int columnX, int yOffset, float screenScaleX, float screenScaleY)
{
    const char* str;
    FormattedText* text;
    UIBox bounds;
    int textWidth = 50;
    int z = 5000;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font(&game_9);
    font_color(SELECT_FROM_UISKIN(CLR_ONLINE_ITEM, CLR_VILLAGON, CLR_ROGUEAGON),
        SELECT_FROM_UISKIN(CLR_ONLINE_ITEM, CLR_VILLAGON, CLR_ROGUEAGON));

    uiBoxDefine(&bounds, (columnX+RING_WD/2-textWidth/2)*screenScaleX, (202-yOffset)*screenScaleY, textWidth*screenScaleX, 30*screenScaleY);
    str = textStd("CurrentColor");
    str = unescapeString(str);
    text = stFormatText(str, 50*screenScaleX, screenScale);
    stDrawFormattedText(bounds, z, text, screenScale);

    uiBoxDefine(&bounds, (columnX+SUPER_COLOR1_OFFSET+((RING_WD+SUPER_COLOR_SPACING-textWidth)/2))*screenScaleX, (202-yOffset)*screenScaleY, textWidth*screenScaleX, 30*screenScaleY);
    str = textStd("TeamColors");
    str = unescapeString(str);
    text = stFormatText(str, 50*screenScaleX, screenScale);
    stDrawFormattedText(bounds, z, text, screenScale);
}

static void drawShowEmblemSelector( float x, float y, float z, float scale)
{
    AtlasTex *check, *checkD;
    AtlasTex * glow   = atlasLoadTexture("costume_button_link_glow.tga");
    int checkColor = CLR_WHITE;
    int checkColorD = CLR_WHITE;
    Entity * e = playerPtr();
    CBox box;

    if( e->pl->hide_supergroup_emblem )
    {
        if (game_state.skin != UISKIN_HEROES)
        {
            check  = atlasLoadTexture("costume_button_link_check_grey.tga");
            checkD = atlasLoadTexture("costume_button_link_check_press_grey.tga");
        }
        else
        {
            check  = atlasLoadTexture("costume_button_link_check.tga");
            checkD = atlasLoadTexture("costume_button_link_check_press.tga");
        }
    }
    else
    {
        if (game_state.skin != UISKIN_HEROES)
        {
            check   = atlasLoadTexture("costume_button_link_off_tintable.tga");
            checkD  = atlasLoadTexture("costume_button_link_off_press_tintable.tga");
        }
        else
        {
            check   = atlasLoadTexture("costume_button_link_off.tga");
            checkD  = atlasLoadTexture("costume_button_link_off_press.tga");
        }
    }

    BuildCBox( &box, x - check->width*scale/2, y - check->width*scale/2, check->width*scale, check->height*scale );

    if( mouseCollision( &box ))
    {
        scale *= 1.1f;
        display_sprite( glow, x - glow->width*scale/2, y - glow->height*scale/2, z-1, scale, scale, CLR_WHITE );

        if( isDown( MS_LEFT ) )
            display_sprite( checkD, x - checkD->width*scale/2, y - checkD->height*scale/2, z, scale, scale, checkColorD );
        else
            display_sprite( check, x - check->width*scale/2, y - check->height*scale/2, z, scale, scale, checkColor );

        if( mouseClickHit( &box, MS_LEFT) )
        {
            e->pl->hide_supergroup_emblem = !e->pl->hide_supergroup_emblem;
            sendHideEmblemChange(e->pl->hide_supergroup_emblem);
        }
        scale /= 1.1f;
    }
    else
        display_sprite( check, x - check->width*scale/2, y - check->height*scale/2, z, scale, scale, checkColor );

    font( &game_9 );
    font_color( CLR_WHITE, CLR_WHITE );
     cprntEx( x + (check->width/2 + 5)*scale, y, z, scale, scale, CENTER_Y, "HideSupergroupEmblem" );
}

//-----------------------------------------------------------------------------------------------------------------
// King of the supercostume functions /////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

void supercostumeMenu()
{
    tailorMenuEx(1);
}

//-----------------------------------------------------------------------------------------------------------------
// Prince of the supercostume functions /////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

void supercostumeTailorMenu(float scaleOverride, int sgMode, float screenScaleX, float screenScaleY)
{
    Entity *e = playerPtr();
    AtlasTex * layover = NULL; //atlasLoadTexture( "_Layout_costume.tga" );
    static int init = FALSE, ht;
    UIBox colorPickerBox;
    int z = 10;
    int sgModeYOffset = sgMode ? -80 : 0;
    static ScrollBar sb = {0};
    float screenScale = MIN(screenScaleX, screenScaleY);

    //drawBackground( layover );
    //drawMenuTitle( 10, 30, 20, "ChooseSuperGroupColors", 1.f, !init );

    if( !gSuperCostume )
        return;

    if( !init )
    {
        init = TRUE;

        // [COH-23111] this was causing graphical flashes.  texEnableThreadedLoading() is never called.
        // Also, it is odd that the threaded loading would only be disabled the first time into this code
        // since other code could re-enable the threaded loading.
        // Seems like if we need it, then it should always disable when this menu is shown, and re-enabled
        // when this menu is hidden.
        //texDisableThreadedLoading();

        //initCostume();
        //supercostume_init();
        costume_setCurrentCostume( e, 1 );

        gZoomed_in = 0;
        zoomAvatar(true, avatar_getDefaultPosition(0,0));
        updateCostume();
    }


    //seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE ); // freeze the player model

    // frame around costume UI
    // use scaleOverride to move this
    if (scaleOverride < 0.1f)
    {
        return;
    }
    uiBoxDefine(&colorPickerBox, 48, 187 + sgModeYOffset, 662, (495 - sgModeYOffset) * scaleOverride);
    drawMenuFrame( R12, colorPickerBox.x*screenScaleX, colorPickerBox.y*screenScaleY, z, colorPickerBox.width*screenScaleX, colorPickerBox.height*screenScaleY, CLR_WHITE, 0x00000088, FALSE );

    // Draw column headers.
    font(&game_14);
    if (scaleOverride == 1.0f)
    {
        UIBox box;
        uiBoxDefine(&box, 277, 168 + sgModeYOffset, 130, 13);
           drawMenuHeader((box.x+box.width/2)*screenScaleX, (box.y+box.height/2)*screenScaleY, z, SELECT_FROM_UISKIN(CLR_WHITE, CLR_YELLOW, CLR_BLUE), SELECT_FROM_UISKIN(0x88b6ffff, CLR_RED, 0x8888ccff), "SuperColor1", screenScale);

        uiBoxDefine(&box, 495, 168 + sgModeYOffset, 140, 13);
        drawMenuHeader((box.x+box.width/2)*screenScaleX, (box.y+box.height/2)*screenScaleY, z, SELECT_FROM_UISKIN(CLR_WHITE, CLR_YELLOW, CLR_BLUE), SELECT_FROM_UISKIN(0x88b6ffff, CLR_RED, 0x8888ccff), "SuperColor2", screenScale);
    }


    // draw button/ color pickers for each part
    uiBoxAlter(&colorPickerBox, UIBAT_SHRINK, UIBAD_ALL, 3);    // account for frame width.  JS: This should be done in drawMenuFrame.
    colorPickerBox.width -= 1;
    colorPickerBox.height -= 1;
    set_scissor(true);

    scissor_dims( colorPickerBox.x*screenScaleX,colorPickerBox.y*screenScaleY, colorPickerBox.width*screenScaleX, colorPickerBox.height*screenScaleY );

    // Label color rings for each column
    if (scaleOverride == 1.0f)
    {
        drawColumnColorRingLabels(PICKER_C1_X, (sb.offset - sgModeYOffset), screenScaleX, screenScaleY);
        drawColumnColorRingLabels(PICKER_C2_X, (sb.offset - sgModeYOffset), screenScaleX, screenScaleY);
    }

    colorPickerBox.y -= sb.offset;
    ht = supercostume_drawAllPickers(e, colorPickerBox, screenScaleX, screenScaleY);
    drawShowEmblemSelector( 325*screenScaleX, (colorPickerBox.y + ht - 20)*screenScaleY, 50, screenScale );
    set_scissor(false);

    if (scaleOverride == 1.0f)
    {
        doScrollBarEx( &sb, colorPickerBox.height, ht, (colorPickerBox.x + colorPickerBox.width + 3), (sgMode ? 109 : 187), 50, 0, &colorPickerBox, screenScaleX, screenScaleY );
    }
}


/* End of File */
