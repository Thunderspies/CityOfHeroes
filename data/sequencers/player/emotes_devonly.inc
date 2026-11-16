#############################################################
## Emotes_DevOnly.inc - Dev Only Emotes
#############################################################
## Interrupts
## <IRQEmotesALL> - Other emotes, basic readies
## <IRQEmotesCombat> - Combat cycles
## <IRQEmotesRighthand> - Right-Handed weapon combat cycles (Broadsword, Mace, etc)
## <IRQEmotesLefthand> - Left-Handed weapon combat cycles (Katana, Archery, etc)
## <IRQEmotesDualhand> - Dual Wield weapon combat cycles (Dual Blades, Dual pistols, Claws, etc)
## <IRQEmotesTwohand> - Two handed weapon combat cycles (Assault Rifle, Stone Mallet, etc)
## <IRQEmotesShield> - Shield combat cycles (Assault Rifle, Stone Mallet, etc)
##
## Members
## <DeathIRQ> - All emotes should be a member of this group
## <AttackIRQ> - All emotes should be a member of this group
## <AttackIRQ> - All emotes should be a member of this group
## <MoveIRQ> - All emotes should be a member of this group
## <ReactIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <HitIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <IRQEmotesALL> - All emotes should be a member of this group UNLESS we want to restrict spamming them (ie. Howls, slaps, etc)

#############################################################
##  Disable PCM Device (Female\Dev Only)
#############################################################

Move Emote_Disable_PCM_Device
	Requires	EMOTE DISABLEPCM
	Priority	2
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NOSIZESCALE CANTMOVE FIXARMREG
	NextMove	Ready
	Interpolate	15

	Type		fem
	  Anim          fem\disable_pcm_device 1
	  PlayFx        POWERS\ItemsAndTempPowers\PCM_Explosion\PCM_Explosion.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Reichsman's Deafening Shout
#############################################################

Move Emote_Reichsman_Attack
	Requires	EMOTE REICHSMAN COMBAT ZEUSATTACK
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Emote_Reichsman_Attack_Post
	Flags		CANTMOVE NoSizeScale

	Type		male
	  Anim          male\states_zeus 1 99
	  PlayFX	V_COV\EnemyPowers\Reichsman\Reichsman_LightningStorm.fx 0
	TEnd
MEnd

Move Emote_Reichsman_Attack_Post
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Emote_Reichsman_Attack_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\states_zeus 100
	TEnd
MEnd

Move Emote_Reichsman_Attack_PostCycle
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\combat_cycle 1 31
	TEnd
MEnd

#############################################################
## Reichsman Captured State
#############################################################

Move Emote_Reichsman_Captured_Pre
	Requires	EMOTE CAPTURED REICHSMAN ONE
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable
	Interpolate	0
	Wind		FLAG_VERTICAL
	Scale		0.25
	NextMove	Emote_Reichsman_Captured_Cycle

	Type		male
	  Anim          male\Reichsman_Captured_Cycle 1 60
	  PlayFX	Scripted\CapturedReichsman\CapturedReichsman.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Reichsman_Captured_Cycle
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle NoSizeScale NotSelectable
	Wind		FLAG_VERTICAL
	Scale		0.25
	NextMove	Emote_Reichsman_Captured_Out

	Type		male
	  Anim          male\Reichsman_Captured_Cycle 1 60
	  PlayFX	Scripted\CapturedReichsman\CapturedReichsman.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move Emote_Reichsman_Captured_Out
	Requires	EMOTE CAPTURED REICHSMAN TWO
	Priority        3
	Interrupts	Emote_Reichsman_Captured_Pre Emote_Reichsman_Captured_Cycle
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable
	NextMove	Emote_Reichsman_Captured_Out_PostCycle

	Type		male
	  Anim          male\Reichsman_Captured_Out 1 140
	TEnd
MEnd

Move Emote_Reichsman_Captured_Out_PostCycle
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale
	NextMove	Emote_Idle2_Cycle

	Type		male
	  Anim          male\emote_VillainLaugh 65 180
	TEnd
MEnd

## CUSTOM 'RIFLE' - POINT ###########################################################

Move Emote_CustomRifle_Point
	Requires	EMOTE CUSTOMWEAPON RIFLE POSE POINT
	Priority	5
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	#Flags		Cycle
	SticksOnChild	TWOHAND
	Scale		0.5

	Type		male
	  Anim          male\shotgun_point 1 100
	TEnd

	Type		fem
	  Anim          fem\shotgun_point 1 100
	TEnd
MEnd


######################
### Walkie Talkie idle   #####
#####################

Move Emote_WalkieTalkie_idle
	Requires	EMOTE CHAT POSE FAR
	Priority	4
	Interrupts	<IRQEmotesALL> <IRQEmotesCombat> #<IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Include sequencers\Members\Emotes_Idle.inc
	#Flags		Cycle
	Scale		0.5

	Type		male
	  Anim          male\walkietalkie 1
	  PlayFx        GENERIC\Walkietalkie.fx  0 Constant
	TEnd
MEnd
####################
### ppd gauntlet look ####
####################
Move Emote_PPD_Gauntlet_Type
	Requires	EMOTE PPD TOUCHSCREEN ONE
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable
	NextMove	Emote_PPD_Gauntlet_Type_Post

	Type		male
	  Anim          male\ppd_stand_type 1 102
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 1 102
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 1 102
	TEnd
MEnd

Move Emote_PPD_Gauntlet_Type_Post
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable

	Type		male
	  Anim          male\ppd_stand_type 204 243
	TEnd

	Type		fem
	  Anim          fem\ppd_stand_type 204 243
	TEnd

	Type		huge
	  Anim          huge\ppd_stand_type 204 243
	TEnd

MEnd



## Abomination - Halloween Event 2011 ###########################################################

Move Abomination_Inactive
	Requires	ENCOUNTER ABOMINATION POSE LOW
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	Flags		Cycle Notselectable cantmove
	CycleMove	Abomination_Inactive_Cycle
	Scale		0.7

	Type		Abomination
	  Anim          huge\emote_abomination_inactive_cycle 1
	TEnd

MEnd

Move Abomination_Inactive_Cycle
	Requires	ENCOUNTER ABOMINATION POSE LOW
	Priority	4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle Notselectable cantmove
	CycleMove	Abomination_Inactive_Cycle
	Scale		0.7

	Type		Abomination
	  Anim          huge\emote_abomination_inactive_cycle 1
	TEnd

MEnd

## Abomination - Halloween Event 2011 ###########################################################

Move Abomination_Activate
	Requires	ENCOUNTER ABOMINATION POSE HIGH
	Priority	4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Abomination_Activate_To_CombatReady
	Flags		Notselectable cantmove
	Scale		0.8

	Type		Abomination
	  Anim          huge\emote_abomination_activate 1
	TEnd

MEnd

Move Abomination_Activate_To_CombatReady
	Requires	ENCOUNTER ABOMINATION POSE HIGH
	Priority	4
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	CombatReady
	Scale           0.50
	Flags		Notselectable cantmove
	Interpolate	20

	Type		Abomination
	  Anim          huge\combat_cycle 1 31
	TEnd

MEnd

 ####################################################
### Signature Stories
 ####################################################

######### Male Arms Behind Back Cycle ####################################################

Move Emote_Male_Stand_Cycle_Pre
	Requires	EMOTE SIGNATURE POSE ONE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Emote_Male_Stand_Cycle
	Flags		NoSizeScale fixarmreg

	Type		male
	  Anim          male\emote_ready_armsback 1 2
	TEnd
MEnd

Move Emote_Male_Stand_Cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale CYCLE fixarmreg

	Type		male
	  Anim          male\emote_ready_armsback 2
	TEnd
MEnd

###### Male Stand to Levitate Cast (one off) #######################################################

Move Emote_Male_Stand_To_Levitate
	Requires	EMOTE SIGNATURE POSE TWO
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Levitate_Cycle_Pre
	Flags		NoSizeScale fixarmreg

	Type		male
	  Anim          male\emote_ready_armsback_to_levitate 1
	TEnd
MEnd

Move Emote_Male_Levitate_Cycle_Pre
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Emote_Male_Levitate_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_levitate_cycle 1 2
	TEnd
MEnd

Move Emote_Male_Levitate_Cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale CYCLE

	Type		male
	  Anim          male\emote_levitate_cycle 2
	TEnd
MEnd

###### Male Levitate To Ready (one off) #######################################################

Move Emote_Male_Levitate_To_Ready
	Requires	EMOTE SIGNATURE POSE THREE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Levitate_To_Ready_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_levitate_to_ready 1
	TEnd
MEnd

Move Emote_Male_Levitate_To_Ready_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd


###### Male CombatReady to Absorb (one off) #######################################################

Move Emote_Male_CombatReady_To_Absorb
	Requires	EMOTE SIGNATURE POSE FOUR
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Absorb_Cycle_Pre
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_combatready_to_onehand_absorb 1
	TEnd
MEnd

Move Emote_Male_Absorb_Cycle_Pre
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Emote_Male_Absorb_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_onehand_absorb_cycle 1 2
	TEnd
MEnd

Move Emote_Male_Absorb_Cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale CYCLE

	Type		male
	  Anim          male\emote_onehand_absorb_cycle 2
	TEnd
MEnd

###### Male Absorb To Hover Absorb (one off) #######################################################

Move Emote_Male_Absorb_To_Hover
	Requires	EMOTE SIGNATURE POSE FIVE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Hover_Absorb_Cycle_Pre
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_onehand_absorb_to_villainstance_handout 1
	TEnd
MEnd

Move Emote_Male_Hover_Absorb_Cycle_Pre
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Emote_Male_Hover_Absorb_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_villainstance_handout_cycle 1 2
	TEnd
MEnd

Move Emote_Male_Hover_Absorb_Cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale CYCLE

	Type		male
	  Anim          male\air_emote_villainstance_handout_cycle 2
	TEnd
MEnd

###### Male Absorb To Ready (one off) #######################################################

Move Emote_Male_Hover_To_Ready
	Requires	EMOTE SIGNATURE POSE SIX
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Hover_To_Ready_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_onehand_absorb_to_ready 1
	TEnd
MEnd

Move Emote_Male_Hover_To_Ready_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.50

	Type		male
	  Anim          male\Skel_ready2 1
	TEnd
MEnd

##############################################################
##############################################################
Move Emote_Male_Ready_To_React
	Requires	EMOTE SIGNATURE POSE SEVEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Ready_To_React_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_ready_to_react 1 55
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\PowerSuckRay.fx 1
	TEnd
MEnd

Move Emote_Male_Ready_To_React_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		1.2

	Type		male
	  Anim          male\emote_ready_to_react 55 112
	TEnd
MEnd
##############################################################
Move Emote_Male_React_To_One_Knee
	Requires	EMOTE SIGNATURE POSE EIGHT
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_React_To_One_Knee_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_react_to_one_knee 1 75
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\BodyStatic.fx 1 CONSTANT

	TEnd
MEnd

Move Emote_Male_React_To_One_Knee_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\BodyStatic.fx 1 CONSTANT
	TEnd
MEnd

##############################################################
Move Emote_Male_One_Knee_To_Float_Pre
	Requires	EMOTE SIGNATURE POSE NINE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_One_Knee_To_Float
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_one_knee_to_float 1 47
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\BodyStatic.fx 1 CONSTANT
	TEnd
MEnd

Move Emote_Male_One_Knee_To_Float
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_One_Knee_To_Float_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_float_react 1 87
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\BodyStatic.fx 1 CONSTANT

	TEnd
MEnd


Move Emote_Male_One_Knee_To_Float_Post
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_One_Knee_To_Float_PostPost
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_float_react 88 100
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Buildup.fx 1 CONSTANT
	TEnd
MEnd

Move Emote_Male_One_Knee_To_Float_PostPost
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale Cycle

	Type		male
	  Anim          male\emote_float_react 100 243
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\BodyShock.fx 1 CONSTANT
	TEnd
MEnd
##############################################################
Move Emote_Male_Float_to_Death
	Requires	EMOTE SIGNATURE POSE TEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Float_to_Death_Post
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_float_to_death 1 24
	  PlayFX	Scripted\SignatureStories\OneWillDie\Statesman\BodySmoke.fx 1
	TEnd
MEnd

Move Emote_Male_Float_to_Death_Post
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Float_to_Death_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_float_to_death 25 165
	TEnd
MEnd

Move Emote_Male_Float_to_Death_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\emote_float_to_death 166 166
	TEnd
MEnd


##############################################################
#### Female Death
##############################################################

Move Emote_Fem_Stand_to_React
	Requires	EMOTE SIGNATURE POSE ELEVEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Fem_Stand_to_React_PostCycle
	Flags		NoSizeScale

	Type		fem
	  Anim          fem\sonic_pain 1 1
	TEnd

MEnd

Move Emote_Fem_Stand_to_React_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		fem
	  Anim          fem\sonic_pain 1
	  playfx	Scripted\SignatureStories\OneWillDie\SigArc6\LosingControl.fx 1 CONSTANT
	TEnd

MEnd

##############################################################

Move Emote_Fem_React_To_Float
	Requires	EMOTE SIGNATURE POSE TWELVE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Fem_React_To_Float_PostCycle
	Flags		NoSizeScale

	Type		fem
	  Anim          fem\emote_react_to_Float 1
	  playfx	Scripted\SignatureStories\OneWillDie\SigArc6\LosingControl.fx 1 CONSTANT
	TEnd

MEnd

Move Emote_Fem_React_To_Float_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		fem
	  Anim          fem\emote_react_float 1
	  playfx	Scripted\SignatureStories\OneWillDie\SigArc6\LosingControl.fx 1 CONSTANT
	TEnd

MEnd

##############################################################

Move Emote_Male_Looking_Up
	Requires	EMOTE SIGNATURE POSE THIRTEEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Looking_Up_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_react_lookup 1 26
	TEnd

MEnd

Move Emote_Male_Looking_Up_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\emote_react_lookup 27 64
	TEnd

MEnd

##############################################################

Move Emote_Male_Looking_Down
	Requires	EMOTE SIGNATURE POSE FOURTEEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Looking_Down_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_react_lookdown 1 22
	TEnd

MEnd

Move Emote_Male_Looking_Down_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\emote_react_lookdown 22 67
	TEnd

MEnd

##############################################################
Move Emote_Citizens_Stand_to_React
	Requires	EMOTE SIGNATURE POSE FIFTEEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Citizens_Stand_to_React_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\sonicpain 1
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

MEnd

Move Emote_Citizens_Stand_to_React_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\sonicpain 1
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

MEnd

##############################################################

Move Emote_Citizens_Drop_To_Knees_React
	Requires	EMOTE SIGNATURE POSE SIXTEEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Citizens_Drop_To_Knees_React_PostCycle
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim          male\emote_psionic_react_drop_to_knees 1 124
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_psionic_react_drop_to_knees 1 124
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

MEnd

Move Emote_Citizens_Drop_To_Knees_React_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle  FIXARMREG

	Type		male
	  Anim          male\emote_psionic_react_drop_to_knees 124 176
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\emote_psionic_react_drop_to_knees 124 176
	  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\CitizenReaction.fx 1 CONSTANT
	TEnd

MEnd

##############################################################

Move Emote_Fem_Hit_To_Float
	Requires	EMOTE SIGNATURE POSE SIXTEEN TWO
	Priority        5
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Fem_Floating_Dead_PostCycle
	Flags		NoSizeScale

	Type		fem
	  Anim          fem\emote_hit_to_Float 1
	  PlayFX	Scripted\SignatureStories\OneWillDie\SigArc6\DeathBlow.fx 1
	TEnd

MEnd

##############################################################

Move Emote_Fem_Floating_DeadEmote_Fem_Floating_Dead
	Requires	EMOTE SIGNATURE POSE SEVENTEEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Fem_Floating_Dead_PostCycle
	Flags		NoSizeScale

	Type		fem
	  Anim          fem\emote_floating_dead 1 1
	#  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\Arrow_Only.fx 1
	TEnd

MEnd

Move Emote_Fem_Floating_Dead_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale		0.5

	Type		fem
	  Anim          fem\emote_floating_dead 1
	#  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\Arrow_Only.fx 1 CONSTANT
	TEnd

MEnd

##############################################################

Move Emote_Fem_Float_to_Death
	Requires	EMOTE SIGNATURE POSE EIGHTEEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Fem_Float_to_Death_PostCycle
	Flags		NoSizeScale

	Type		fem
	  Anim          fem\emote_float_to_dead 1 26
	#  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\Arrow_Only.fx 1 CONSTANT
	TEnd

MEnd

Move Emote_Fem_Float_to_Death_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		fem
	  Anim          fem\emote_float_to_dead 26 26
	#  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\Arrow_Only.fx 1 CONSTANT
	TEnd

MEnd

##############################################################

Move Emote_Male_Holding_Fem
	Requires	EMOTE SIGNATURE POSE NINETEEN
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Male_Holding_Fem_PostCycle
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim          male\emote_holding_Fem 1 1
	TEnd

MEnd

Move Emote_Male_Holding_Fem_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle  FIXARMREG
	Scale		0.5

	Type		male
	  Anim          male\emote_holding_Fem 1
	TEnd

MEnd

##############################################################

Move Emote_Fem_Being_Held
	Requires	EMOTE SIGNATURE POSE TWENTY
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_Fem_Being_Held_PostCycle
	Flags		NoSizeScale

	Type		fem
	  Anim          fem\PL_OutCold 1 3
	#  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\Arrow_Only.fx 1 CONSTANT
	TEnd

MEnd

Move Emote_Fem_Being_Held_PostCycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		fem
	  Anim          fem\PL_OutCold 1 3
	#  PlayFx	Scripted\SignatureStories\OneWillDie\SigArc6\Arrow_Only.fx 1 CONSTANT
	TEnd

MEnd

###############################################


Move Emote_PandoraBoxOpen
	Requires	EMOTE SIGNATURE POSE TWENTYONE
	Priority        4
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Include sequencers\Members\Encounters_Scripted.inc
	NextMove	Emote_PandoraBoxOpen_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_pandorabox_open 1 150
	TEnd

MEnd

Move Emote_PandoraBoxOpen_Cycle
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle

	Type		male
	  Anim          male\emote_pandorabox_open 150
	TEnd

MEnd

###############################################
