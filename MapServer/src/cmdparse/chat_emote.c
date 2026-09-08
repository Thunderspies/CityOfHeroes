/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "chat_emote.h"
#include "entity/entity.h"
#include "entity/entityRef.h"
#include "TeamReward.h"
#include "player/badges_server.h"
#include "language/langServerUtil.h"
#include "entity/character_eval.h"
#include "gameComm/svr_chat.h"
#include <utilitieslib/utils/eval.h>

SHARED_MEMORY EmoteAnims g_EmoteAnims;

bool EmoteAnimFinalProcess(ParseTable* pti, void* structptr, bool shared_memory)
{
    EmoteAnims* anims = (EmoteAnims*)structptr;
    int i;

    for (i = 0; i < eaSize(&anims->ppAnims); i++)
    {
        const EmoteAnim *anim = anims->ppAnims[i];

        if (anim->pchRequires)
            chareval_Validate(anim->pchRequires, "defs/emotes.def");
        if (anim->pchPowerRequires)
            chareval_Validate(anim->pchPowerRequires, "defs/emotes.def");
    }

    return true;
}

// Costume change emotes
bool gIsCCemote = 0;
bool gDetectedCCemote = 0;

/**********************************************************************func*
 * CCEmoteCatch
 *
 * Used to detect costume change emotes
 */
void CCEmoteCatch(EvalContext *pcontext)
{
    eval_IntPush(pcontext, 1);
    gDetectedCCemote = 1;
}

//----------------------------------------
//  check if the ent has the permissions
// to run this emote anim. 
// NOTE: assumes anim name matches already.
//----------------------------------------
bool chatemote_CanDoEmote(const EmoteAnim *anim, Entity *e)
{
    bool res = true;

    //If this is a pet, check the owner's permissions instead of the pets
    Entity *whoWeCheck = (e && e->erOwner) ? erGetEnt(e->erOwner) : e;

    EvalContext *context = chareval_Context();

    gDetectedCCemote = 0;
    //Only players and their pets get checked for permissions
    if( whoWeCheck && anim && (ENTTYPE_PLAYER == ENTTYPE(whoWeCheck)) ) 
    {
        //Dev access required?
        if ( anim->bDevOnly && (whoWeCheck->access_level <= ACCESS_USER) )
        {
            res = false;
        }

        //A Badge required?
        if( anim->pchRequiredBadge && !badge_OwnsBadge( whoWeCheck, anim->pchRequiredBadge ) )
        {
            res = false;
        }

        //A reward token required?
        if( anim->pchRequiredToken && !getRewardToken( whoWeCheck,anim->pchRequiredToken ) )
        {
            res = false;
        }

        // This is mega ugly.  To detect costume change emotes we hook an extra function into
        // the standard character evaluator.  The presence or absence of a call to the associated
        // callback will determine if this is a costume change emote
        if (context)
        {
            eval_RegisterFunc(context, "ccemote?", CCEmoteCatch, 0, 1);
        }

        //A requires?
        if (anim->pchRequires && whoWeCheck->pchar && !chareval_Eval(whoWeCheck->pchar, anim->pchRequires, "defs/emotes.def"))
        {
            res = false;
        }

        if (context)
        {
            eval_UnregisterFunc(context, "ccemote?");
        }

        if (gIsCCemote != gDetectedCCemote)
        {
            res = false;
        }
    }
    
    // ----------
    // finally
    
    return res;
}

bool matchEmoteName( const char * displayName, const char * string )
{
    char translation_key[1024]; 

    if( !displayName || !string )
        return 0;
    if( stricmp( displayName, string ) == 0 )
        return 1;

    strcpy( translation_key, displayName );
    strcat( translation_key, "EmoteText" );

    if( stricmp( localizedPrintf(0,translation_key), string ) == 0 )
            return 1;

    return 0;

}

/* End of File */
