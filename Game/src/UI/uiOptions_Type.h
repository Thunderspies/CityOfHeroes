/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 *
 * Module Description:
 *
 *
 ***************************************************************************/
#ifndef UIOPTIONS_TYPE_H
#define UIOPTIONS_TYPE_H

typedef enum EUserOptions
{
	// ONLY ADD TO END OF LIST
	kUO_NotAnOption,
	kUO_MouseSpeed,		//F32 
	kUO_SpeedTurn,		//F32
	kUO_MouseInvert,	
	kUO_ChatFade,
	kUO_Chat1Fade,
	kUO_Chat2Fade,
	kUO_Chat3Fade,
	kUO_Chat4Fade,
	kUO_CompassFade,
	// ONLY ADD TO END OF LIST
	kUO_UseToolTips,
	kUO_AllowProfanity,
	kUO_ShowBallons,
	kUO_DeclineGifts, 
	kUO_DeclineTeamGifts, 
	kUO_PromptTeamTeleport,
	kUO_ShowPets, 
	kUO_ShowSalvage, 
	kUO_WebHideBasics,
	kUO_WebHideStats,
	kUO_WebHidePowers,
	// ONLY ADD TO END OF LIST
	kUO_WebHideBadges,
	kUO_WebHideFriends, 
	kUO_WindowFade, 
	kUO_LogChat,
	kUO_HideHeader, 
	kUO_HideButtons, 
	kUO_ClickToMove, 
	kUO_DisableDrag, 
	kUO_ShowPetBuffs, 
	kUO_PreventPetIconDrag, 
	// ONLY ADD TO END OF LIST
	kUO_ShowPetControls, 
	kUO_AdvancedPetControls, 
	kUO_ChatDisablePetSay, 
	kUO_ChatEnablePetTeamSay, 
	kUO_TeamComplete, //2
	kUO_DisablePetNames,
	kUO_HidePlacePrompt,
	kUO_HideDeletePrompt,
	kUO_HideDeleteSalvagePrompt, 
	kUO_HideDeleteRecipePrompt, 
	// ONLY ADD TO END OF LIST
	kUO_HideInspirationFull,
	kUO_HideSalvageFull,
	kUO_HideRecipeFull,
	kUO_HideEnhancementFull,
	kUO_ShowEnemyTells,
	kUO_ShowEnemyBroadcast,
	kUO_HideEnemyLocal,
	kUO_HideCoopPrompt,
	kUO_StaticColorsPerName,
	// ONLY ADD TO END OF LIST
	kUO_ContactSort, //4 
	kUO_RecipeHideMissingParts, 
	kUO_RecipeHideUnowned,
	kUO_RecipeHideMissingPartsBench, 
	kUO_RecipeHideUnownedBench,
	kUO_DeclineSuperGroupInvite, 
	kUO_DeclineTradeInvite,
	kUO_CamFree,
	kUO_ToolTipDelay, // F32
	kUO_ShowArchetype, //3 
	// ONLY ADD TO END OF LIST
	kUO_ShowSupergroup,
	kUO_ShowPlayerName,
	kUO_ShowPlayerBars,
	kUO_ShowVillainName,
	kUO_ShowVillainBars,
	kUO_ShowPlayerReticles,
	kUO_ShowVillainReticles,
	kUO_ShowAssistReticles,
	kUO_ShowOwnerName,
	// ONLY ADD TO END OF LIST
	kUO_MousePitchOption, // 3
	kUO_MapOptions, // 8
	kUO_BuffSettings, //32
	kUO_ChatBubbleColor1, // 32
	kUO_ChatBubbleColor2, // 32 
	kUO_ChatFontSize, // 5
	kUO_ReverseMouseButtons,
	kUO_DisableCameraShake,
	kUO_DisableMouseScroll,	
	kUO_LogPrivateMessages,
	// ONLY ADD TO END OF LIST
	kUO_ShowPlayerRating, // 3
	kUO_DisableLoadingTips,
	kUO_MouseScrollSpeed,
	kUO_EnableJoystick,
	kUO_FadeExtraTrays,
	kUO_OptionArchitectNav,
	kUO_ArchitectTips,
	kUO_ArchitectAutoSave,
	kUO_NoXP,
	kUO_BlockArchitectComments,
	// ONLY ADD TO END OF LIST
	kUO_TeamLevelAbove, // 6
	kUO_TeamLevelBelow, // 6
	kUO_DisableEmail,   
	kUO_FriendSGEmailOnly,
	kUO_NoXPExemplar,
	kUO_PopHelpDisable,
	kUO_HideFeePrompt,
	kUO_HideUsefulSalvagePrompt, 
	kUO_ForcedPopHelpDisable,
	kUO_GMailFriendOnly,
	// ONLY ADD TO END OF LIST
	kUO_UseOldTeamUI,
	kUO_HideUnclaimableCerts,
	kUO_BlinkCertifications,
	kUO_VoucherPrompt,
	kUO_NewCertificationPrompt,
	kUO_HideUnslotPrompt,
	kUO_MapOptionRevision,
	kUO_MapOptions2,
	kUO_HideOpenSalvagePrompt, 
	kUO_HideLoyaltyTreeAccessButton,
	// ONLY ADD TO END OF LIST
	kUO_HideStoreAccessButton,
	kUO_AutoFlipSuperPackCards,
	kUO_HideConvertConfirmPrompt,
	kUO_HideStorePiecesState, // 3 BITS
	kUO_CursorScale, // F32
	kUO_ShowTimer, // Power Recast Timer
	// ADD HERE
// NOTE: Adding options to the list (and defining them in game_options array in uiOptions_Type.c will take care of a slash command, save to file, and clientside networking,
//		 However you will need to take care of serverside netowrking ( entserver.c and parseClientInput.c ) and database saving ( containerloadsave.c )

	kUO_OptionTotal, 
}EUserOptions;


typedef struct UserOption
{
	int id;
	char *pchDisplayName;
	int	iVal;
	F32 fVal;
	int doNotWrite;
	int bitSize;
	void *data;
	int delaySave;
}UserOption;


#endif //UIOPTIONS_TYPE_H
