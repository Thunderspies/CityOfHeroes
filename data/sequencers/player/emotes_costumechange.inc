#############################################################
## Emotes.inc - Emotes
#############################################################
## Interrupts
## <EmotesALL> - Other emotes, basic readies
## <EmotesCombat> - Combat cycles
## <EmotesRighthand> - Right-Handed weapon combat cycles
## <EmotesRighthand> - Left-Handed weapon combat cycles
## <EmotesRighthand> - Dual Wield weapon combat cycles
##
## Members
## <DeathIRQ> - All emotes should be a member of this group
## <AttackIRQ> - All emotes should be a member of this group
## <FLYAttackAll> - All emotes should be a member of this group
## <MoveIRQ> - All emotes should be a member of this group
## <ReactIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <HitIRQ> - Emotes should be a member of this group unless we specifically don't want them interruptible by being hit
## <EmotesALL> - All emotes should be a member of this group UNLESS we want to restrict spamming them (ie. Howls, slaps, etc)

#############################################################
## Costume Change - Backflip
#############################################################

Move Emote_CostumeChange_Backflip
	Requires	EMOTE CCCHANGE BACKFLIP
	Priority	13
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Nextmove	Emote_CostumeChange_Backflip_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Emote_Backflip 1 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 10 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Backflip_Post
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Ready_2Hips
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Emote_Backflip 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Backflip
	Requires	FLY EMOTE CCCHANGE BACKFLIP
	Priority	24
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Nextmove	A_Emote_CostumeChange_Backflip_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_backflip_air 1 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 10 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Backflip_Post
	Requires	FLY
	CycleMove	A_Emote_CostumeChange_Backflip_PostCycle
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets            CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_backflip_air 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Backflip_PostCycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE
	NextMove	PreFall

	Type		male
	  Anim          male\emote_Backflip_air_cycle 1 53
	TEnd
MEnd

#############################################################

Move Emote_Shield_CostumeChange_Backflip
	Requires	EMOTE CCCHANGE BACKFLIP SHIELD
	Priority	24
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Nextmove	Emote_Shield_CostumeChange_Backflip_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_backflip_shield 1 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 10 CONSTANT
	TEnd
MEnd

Move Emote_Shield_CostumeChange_Backflip_Post
	Include sequencers\Members\Emotes_Idle.inc
	Nextmove	Shield_Ready
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_backflip_shield 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 0 CONSTANT
	TEnd
MEnd

##############################################################

Move A_Emote_Shield_CostumeChange_Backflip
	Requires	FLY EMOTE CCCHANGE BACKFLIP SHIELD
	Priority        35
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_Shield_CostumeChange_Backflip_Post
	Sets            CANTMOVE
	Flags		FIXARMREG NoSizeScale

	Type		male
	  Anim          male\emote_backflip_shield_air 1 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 10 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_Backflip_Post
	Requires	FLY
	CycleMove	A_Emote_Shield_CostumeChange_Backflip_PostCycle
	NextMove	PreFall
	Flags		Cycle NoSizeScale FIXARMREG
	Sets            CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_backflip_shield_air 31
	  PlayFX	Emotes\CostumeChanges\Backflip\Backflip.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_Backflip_PostCycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	NextMove	PreFall

	Type		male
	  Anim          male\emote_backflip_shield_air_cycle 1 53
	TEnd
MEnd

#############################################################
## Costume Change - Evil Laugh
#############################################################

Move Emote_CostumeChange_VillainLaugh
	Requires	EMOTE CCCHANGE VILLAINLAUGH
	Priority	13
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_CostumeChange_VillainLaugh_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_VillainLaugh 1 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_VillainLaugh_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready_2Hips_Bri
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_VillainLaugh 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_VillainLaugh
	Requires	FLY EMOTE CCCHANGE VILLAINLAUGH
	Priority	24
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_VillainLaugh_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_villainlaugh_air 1 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_VillainLaugh_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_CostumeChange_VillainLaugh_PostCycle
	NextMove	PreFall
	Flags		Cycle
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_villainlaugh_air 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_VillainLaugh_PostCycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle
	NextMove	PreFall

	Type		male
	  Anim          male\emote_villainlaugh_air_cycle 1
	TEnd
MEnd

#############################################################

Move Emote_Shield_CostumeChange_VillainLaugh
	Requires	EMOTE SHIELD CCCHANGE VILLAINLAUGH
	Priority	26
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_Shield_CostumeChange_VillainLaugh_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_villainlaugh_shield 1 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Shield_CostumeChange_VillainLaugh_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Ready_2Hips_Bri
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_villainlaugh_shield 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Shield_Emote_CostumeChange_VillainLaugh
	Requires	FLY SHIELD EMOTE CCCHANGE VILLAINLAUGH
	Priority	37
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Shield_Emote_CostumeChange_VillainLaugh_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_villainlaugh_shield_air 1 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

Move A_Shield_Emote_CostumeChange_VillainLaugh_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Shield_Emote_CostumeChange_VillainLaugh_PostCycle
	NextMove	PreFall
	Flags		Cycle
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_villainlaugh_shield_air 75
	  PlayFX	Emotes\CostumeChanges\Laugh\LaughSmoke.fx 0 CONSTANT
	TEnd
MEnd

Move A_Shield_Emote_CostumeChange_VillainLaugh_PostCycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		Cycle
	NextMove	PreFall

	Type		male
	  Anim          male\emote_villainlaugh_shield_air_cycle 1
	TEnd
MEnd

#############################################################
## Costume Change - Howl
#############################################################

Move Emote_CostumeChange_Howl
	Requires	EMOTE CCCHANGE HOWL
	Priority	13
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_CostumeChange_Howl_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 1 25
	  PlayFX	Emotes\CostumeChanges\Howl\Smoke_Howl.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Howl_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_Howl_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 25
	  PlayFX	Emotes\CostumeChanges\Howl\Smoke_Howl.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Howl_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale		0.50

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Howl
	Requires	FLY EMOTE CCCHANGE HOWL
	Priority	24
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Howl_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 1 25
	  PlayFX	Emotes\CostumeChanges\Howl\Smoke_Howl.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Howl_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_C_flycycle
	NextMove	PreFall
	Flags		Cycle NOSIZESCALE
	Sets		CANTMOVE COSTUMECHANGE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 25
	  PlayFX	Emotes\CostumeChanges\Howl\Smoke_Howl.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Costume Change - Salute
#############################################################

Move Emote_CostumeChange_Salute
	Requires	EMOTE CCCHANGE SALUTE
	Priority	13
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_CostumeChange_Salute_Post
	Sets		CANTMOVE
	Flags		FIXARMREG

	Type		male
	  Anim          male\salute 1 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Salute_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_Salute_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		FIXARMREG

	Type		male
	  Anim          male\salute 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Salute_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\thumbsup 99 100
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Salute
	Requires	FLY EMOTE CCCHANGE SALUTE
	Priority	24
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Salute_Post
	Sets		CANTMOVE
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_salute_air 1 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Salute_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_CostumeChange_Salute_PostCycle
	NextMove	PreFall
	Flags		Cycle NOSIZESCALE FIXARMREG
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_salute_air 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Salute_PostCycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	NextMove	PreFall

	Type		male
	  Anim          male\emote_salute_air_cycle 1
	TEnd
MEnd

#############################################################

Move Emote_Shield_CostumeChange_Salute
	Requires	EMOTE SHIELD CCCHANGE SALUTE
	Priority	26
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_Shield_CostumeChange_Salute_Post
	Sets		CANTMOVE
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_salute_shield 1 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Shield_CostumeChange_Salute_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Shield_CostumeChange_Salute_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_salute_shield 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_Shield_CostumeChange_Salute_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		CYCLE FIXARMREG

	Type		male
	  Anim          male\emote_salute_shield 102 102
	TEnd
MEnd

#############################################################

Move A_Emote_Shield_CostumeChange_Salute
	Requires	FLY EMOTE SHIELD CCCHANGE SALUTE
	Priority	37
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_Shield_CostumeChange_Salute_Post
	Sets		CANTMOVE
	Flags		FIXARMREG

	Type		male
	  Anim          male\emote_salute_shield_air 1 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_Salute_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	A_Emote_Shield_CostumeChange_Salute_PostCycle
	NextMove	PreFall
	Flags		Cycle FIXARMREG
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_salute_shield_air 24
	  PlayFX	Emotes\CostumeChanges\Salute\BodyGlow_Salute.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_Shield_CostumeChange_Salute_PostCycle
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	Flags		CYCLE FIXARMREG
	NextMove	PreFall

	Type		male
	  Anim          male\emote_salute_shield_air_cycle 1
	TEnd
MEnd

#############################################################
## Vanguard Sigil
#############################################################

Move Emote_CostumeChange_VanguardSigil
	Requires	EMOTE CCCHANGE MAGIC GROUNDSUMMON ONE
	Priority        15
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_VanguardSigil_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 60
	  PlayFX	Emotes\CostumeChanges\MagicSigil\Vanguard_MagicSigil.fx 1 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_VanguardSigil_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        CombatBlend
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 60
	  PlayFX	Emotes\CostumeChanges\MagicSigil\Vanguard_MagicSigil.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_VanguardSigil
	Requires	FLY EMOTE CCCHANGE MAGIC GROUNDSUMMON ONE
	Priority        26
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_VanguardSigil_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\teleport 1 30
	  PlayFX	Emotes\CostumeChanges\MagicSigil\Vanguard_MagicSigil.fx 24 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_VanguardSigil_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	C_flyready
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\teleport 31
	  PlayFX	Emotes\CostumeChanges\MagicSigil\Vanguard_MagicSigil.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Costume Change - Cast
#############################################################

Move Emote_CostumeChange_Cast_One
	Requires	EMOTE CCCHANGE CAST ONE
	Priority	14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_CostumeChange_Cast_One_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\magic_L 1 35
	  PlayFX	Emotes\CostumeChanges\Cast\SmokeCast.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Cast_One_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	postReady_back
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\magic_L 35
	  PlayFX	Emotes\CostumeChanges\Cast\SmokeCast.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Cast_One
	Requires	FLY EMOTE CCCHANGE CAST ONE
	Priority	25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Cast_One_Post
	Sets		CANTMOVE

	Type		male
	  Anim          male\air_magicL 1 35
	  PlayFX	Emotes\CostumeChanges\Cast\SmokeCast.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Cast_One_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_CFlyReady
	NextMove	PreFall
	Flags		Cycle
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_magicL 35
	  PlayFX	Emotes\CostumeChanges\Cast\SmokeCast.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Lightning Costume Change
#############################################################

Move Emote_CostumeChange_Lightning
	Requires	EMOTE CCCHANGE LIGHTNINGROD ONE
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_Lightning_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\glory 1 45
	  PlayFX	Emotes\CostumeChanges\Lightning\LightningBlast.fx
	TEnd
MEnd

Move Emote_CostumeChange_Lightning_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Lazy_Combat_Blend
	Flags		NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\glory 46
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Lightning
	Requires	FLY EMOTE CCCHANGE LIGHTNINGROD ONE
	Priority        25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Lightning_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_glory 1 45
	  PlayFX	Emotes\CostumeChanges\Lightning\LightningBlast.fx
	TEnd
MEnd

Move A_Emote_CostumeChange_Lightning_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_C_flycycle_Half
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_glory 46
	TEnd
MEnd

#############################################################
## Spin Costume Change
#############################################################

Move Emote_CostumeChange_Spin_Pre
	Requires	EMOTE CCCHANGE SPIN ONE
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_Spin
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spin 1 45
	  PlayFX	Emotes\CostumeChanges\Tornado\Tornado.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Spin
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        JumpPost
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spin 45
	  PlayFX	Emotes\CostumeChanges\Tornado\Tornado.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Spin_Pre
	Requires	FLY EMOTE CCCHANGE SPIN ONE
	Priority        25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Spin
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\spin 1 45
	  PlayFX	Emotes\CostumeChanges\Tornado\Tornado.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Spin
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Preflyready
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\spin 45
	  PlayFX	Emotes\CostumeChanges\Tornado\Tornado.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Presto Chango
#############################################################

Move Emote_CostumeChange_PrestoChango
	Requires	EMOTE CCCHANGE SKYSUMMON ONE
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_PrestoChango_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 30
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 24 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_PrestoChango_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        CombatBlend
	Priority        0
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 31
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_PrestoChango
	Requires	FLY EMOTE CCCHANGE SKYSUMMON ONE
	Priority        25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_PrestoChango_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 1 30
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 24 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_PrestoChango_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_CFlyReady
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_skysummon 31
	  PlayFX	Emotes\CostumeChanges\PrestoChango\PrestoChango.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Transform
#############################################################

Move Emote_CostumeChange_Peacebringer
	Requires	EMOTE CCCHANGE PEACEBRINGER SHAPESHIFT
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_Peacebringer_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 1 65
	  PlayFX	POWERS\CosmicPowers\PeaceBringer\BlasterShapshift\BlasterChange.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Peacebringer_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        CombatBlend
	Priority        0
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 65
	  PlayFX	POWERS\CosmicPowers\PeaceBringer\BlasterShapshift\BlasterChange.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Peacebringer
	Requires	FLY EMOTE CCCHANGE PEACEBRINGER SHAPESHIFT
	Priority        25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Peacebringer_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_pre_poison 1 65
	  PlayFX	POWERS\CosmicPowers\PeaceBringer\BlasterShapshift\BlasterChange.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Peacebringer_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_CFlyReady
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_pre_poison 65
	  PlayFX	POWERS\CosmicPowers\PeaceBringer\BlasterShapshift\BlasterChange.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Warshade Transform
#############################################################

Move Emote_CostumeChange_Warshade
	Requires	EMOTE CCCHANGE WARSHADE SHAPESHIFT
	Priority        14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_Warshade_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 1 65
	  PlayFX	POWERS\CosmicPowers\Nictus\BlasterShapshift\BlasterChange.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Warshade_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        CombatBlend
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 65
	  PlayFX	POWERS\CosmicPowers\Nictus\BlasterShapshift\BlasterChange.fx 0 CONSTANT
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Warshade
	Requires	FLY CCCHANGE WARSHADE SHAPESHIFT
	Priority        25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Warshade_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_pre_poison 1 65
	  PlayFX	POWERS\CosmicPowers\Nictus\BlasterShapshift\BlasterChange.fx 24 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Warshade_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_CFlyReady
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_pre_poison 65
	  PlayFX	POWERS\CosmicPowers\Nictus\BlasterShapshift\BlasterChange.fx 24 CONSTANT
	TEnd
MEnd


## End Game Reward Costume Change Emotes
#############################################################

#############################################################
## Reward - IceBlock Costume Change
#############################################################

Move Emote_CostumeChange_IceBlock_Pre
	Requires	EMOTE CCCHANGE LIGHTNINGROD ONE ALTERNATE
	Priority        15
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_IceBlock
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 30
	  PlayFX	Emotes\CostumeChanges\Ice\GlacierEmote.fx 0 constant
	TEnd

MEnd

Move Emote_CostumeChange_IceBlock
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_CostumeChange_IceBlock_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 30 75
	  PlayFX	Emotes\CostumeChanges\Ice\GlacierEmote.fx 0 constant
	TEnd
MEnd

Move Emote_CostumeChange_IceBlock_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Lazy_Combat_Blend
	Flags		NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\ground_summon 76
	  PlayFX	Emotes\CostumeChanges\Ice\GlacierEmote.fx 0 constant
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_IceBlock_Pre
	Requires	FLY EMOTE CCCHANGE LIGHTNINGROD ONE ALTERNATE
	Priority        26
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_IceBlock_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_Rapidboil 1 35
	  PlayFX	Emotes\CostumeChanges\Ice\GlacierEmote.fx 0 constant
	TEnd

MEnd

Move A_Emote_CostumeChange_IceBlock_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	C_flyready
	NextMove	PreFall
	Sets		CANTMOVE COSTUMECHANGE
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_emote_Rapidboil 35
	  PlayFX	Emotes\CostumeChanges\Ice\GlacierEmote.fx 0 constant
	TEnd

MEnd

#############################################################
## Reward - Rock Block Costume Change
#############################################################

Move Emote_CostumeChange_RockBlock_Pre
	Requires	EMOTE CCCHANGE MAGIC GROUNDSUMMON ONE ALTERNATE
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_RockBlock
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 1 30
	TEnd
MEnd

Move Emote_CostumeChange_RockBlock
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Emote_CostumeChange_RockBlock_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 30 60
	  PlayFX	Emotes\CostumeChanges\Stone\EarthEmote.fx 0 constant
	TEnd
MEnd

Move Emote_CostumeChange_RockBlock_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        CombatBlend
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\ground_summon 60
	  PlayFX	Emotes\CostumeChanges\Stone\EarthEmote.fx 0 constant
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_RockBlock_Pre
	Requires	FLY EMOTE CCCHANGE MAGIC GROUNDSUMMON ONE ALTERNATE
	Priority        27
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_RockBlock_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_emote_Rapidboil 1 35
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 1 35
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 1 35
	TEnd

MEnd

Move A_Emote_CostumeChange_RockBlock_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	C_flyready
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_emote_Rapidboil 35
	  PlayFX	Emotes\CostumeChanges\Stone\EarthEmote_Flight.fx 0 constant
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 35
	  PlayFX	Emotes\CostumeChanges\Stone\EarthEmote_Flight.fx 0 constant
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 35
	  PlayFX	Emotes\CostumeChanges\Stone\EarthEmote_Flight.fx 0 constant
	TEnd
MEnd

#############################################################
## Reward - Fireworks - Costume Change
#############################################################

Move Emote_CostumeChange_Fireworks
	Requires	EMOTE CCCHANGE SKYSUMMON ONE ALTERNATE
	Priority        15
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_Fireworks_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 30
	TEnd
MEnd

Move Emote_CostumeChange_Fireworks_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        CombatBlend
	Priority        0
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 31
	  PlayFX	Emotes\CostumeChanges\Fireworks\ConfettiEmote.fx 0 constant
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Fireworks
	Requires	FLY EMOTE CCCHANGE SKYSUMMON ONE ALTERNATE
	Priority        26
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_Fireworks_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 1 30
	TEnd
MEnd

Move A_Emote_CostumeChange_Fireworks_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_CFlyReady
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_skysummon 31
	  PlayFX	Emotes\CostumeChanges\Fireworks\ConfettiEmote.fx 0 constant
	TEnd
MEnd

## Reward - Energy - Costume Change ###########################################################

Move Emote_Energy_Pre
	Requires	EMOTE CCCHANGE SHIFT ONE ALTERNATE
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_Energy_Cycle
	Flags		NoSizeScale
	SticksOnChild	EMOTE SHIFT

	Type		male
	  Anim          male\flameaura 1 55
	playfx		Emotes\CostumeChanges\Energy\EnergyEmote.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\fireaura 1 55
	playfx		Emotes\CostumeChanges\Energy\EnergyEmote.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\fireaura 1 55
	playfx		Emotes\CostumeChanges\Energy\EnergyEmote.fx 0 constant
	TEnd

MEnd

Move Emote_Energy_Cycle
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_Energy_Post
	Flags		NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_dimension_shift 1 20
	TEnd

MEnd

Move Emote_Energy_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\flameaura 55
	TEnd

	Type		huge
	  Anim          huge\fireaura 55
	TEnd

	Type		fem
	  Anim          fem\fireaura 55
	TEnd
MEnd

## AIR Dimension Shift ###########################################################

Move A_Emote_Energy_Pre
	Requires	FLY EMOTE CCCHANGE SHIFT ONE ALTERNATE
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_Energy_Cycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 1 55
	playfx		Emotes\CostumeChanges\Energy\EnergyEmote.fx 0 constant
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 1 55
	playfx		Emotes\CostumeChanges\Energy\EnergyEmote.fx 0 constant
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 1 55
	playfx		Emotes\CostumeChanges\Energy\EnergyEmote.fx 0 constant
	TEnd

MEnd

Move A_Emote_Energy_Cycle
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove	A_Emote_Energy_Post
	Flags		NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\emote_dimension_shift 1
	TEnd

MEnd

Move A_Emote_Energy_Post
	Requires	FLY
	Priority	1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        FlyReady
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_fireaura 55
	TEnd

	Type		huge
	  Anim          huge\air_fireaura 55
	TEnd

	Type		fem
	  Anim          fem\air_fireaura 55
	TEnd
MEnd

## Reward - Nuke - Costume Change ###########################################################

Move Emote_CostumeChange_Nuke_Pre
	Requires	EMOTE CCCHANGE REWARD TWO ALTERNATE
	Priority        5
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	Flags		NoSizeScale
	NextMove	Emote_CostumeChange_Nuke
	Sets            CANTMOVE

	Type		male
	  Anim          male\emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Nuke\NewEmote_Pre.fx
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Nuke\NewEmote_Pre.fx
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Nuke\NewEmote_Pre.fx
	TEnd

MEnd

Move Emote_CostumeChange_Nuke
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_Nuke_Post
	Sets            CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\Nuke\NukeEmote.fx
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\Nuke\NukeEmote.fx
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\Nuke\NukeEmote.fx
	TEnd
MEnd

Move Emote_CostumeChange_Nuke_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        Ready
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\emote_Rapidboil 71
	TEnd

	Type		fem
	  Anim          fem\emote_Rapidboil 71
	TEnd

	Type		huge
	  Anim          fem\emote_Rapidboil 71
	TEnd
MEnd

## AIR Fur Explode ###########################################################

Move A_Emote_CostumeChange_Nuke_Pre
	Requires	FLY EMOTE CCCHANGE REWARD TWO ALTERNATE
	Priority        16
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Nuke
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Nuke\NewEmote_Pre.fx
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Nuke\NewEmote_Pre.fx
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 1 65
	  PlayFX	Emotes\CostumeChanges\Nuke\NewEmote_Pre.fx
	TEnd

MEnd

Move A_Emote_CostumeChange_Nuke
	Requires	FLY
	Priority        1
	Include sequencers\Members\Emotes_Idle_Air.inc
	NextMove        A_Emote_CostumeChange_Nuke_Post
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale
	Scale		0.5

	Type		male
	  Anim          male\air_emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\Nuke\NukeEmote.fx
	TEnd

	Type		fem
	  Anim          male\air_emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\Nuke\NukeEmote.fx
	TEnd

	Type		huge
	  Anim          male\air_emote_Rapidboil 66 70
	  PlayFX	Emotes\CostumeChanges\Nuke\NukeEmote.fx
	TEnd
MEnd

Move A_Emote_CostumeChange_Nuke_Post
	Include sequencers\Members\Emotes_Idle_Air.inc
	Requires	FLY
	Flags		NoSizeScale
	NextMove        Prefall

	Type		male
	  Anim          male\air_emote_Rapidboil 71
	TEnd
MEnd

#############################################################
## Reward - Fireworks - Costume Change
#############################################################

Move Emote_CostumeChange_MurderofCrows
	Requires	EMOTE CCCHANGE SKYSUMMON THREE ALTERNATE
	Priority        15
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        Emote_CostumeChange_MurderofCrows_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 1 30
	  PlayFX	Emotes\CostumeChanges\Crows\CrowsEmote.fx 0 constant
	TEnd
MEnd

Move Emote_CostumeChange_MurderofCrows_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove        CombatBlend
	Priority        0
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NoSizeScale

	Type		male
	  Anim          male\sky_summon 31
	  PlayFX	Emotes\CostumeChanges\Crows\CrowsEmote.fx 0 constant
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_MurderofCrows
	Requires	FLY EMOTE CCCHANGE SKYSUMMON THREE ALTERNATE
	Priority        26
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove        A_Emote_CostumeChange_MurderofCrows_Post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_skysummon 1 30
	  PlayFX	Emotes\CostumeChanges\Crows\CrowsEmote.fx 0 constant
	TEnd
MEnd

Move A_Emote_CostumeChange_MurderofCrows_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_CFlyReady
	NextMove	PreFall
	Flags		Cycle NoSizeScale
	Sets		CANTMOVE COSTUMECHANGE

	Type		male
	  Anim          male\air_skysummon 31
	  PlayFX	Emotes\CostumeChanges\Crows\CrowsEmote.fx 0 constant
	TEnd
MEnd

#############################################################
## Costume Change - Inner Will
#############################################################

Move Emote_CostumeChange_InnerWill
	Requires	EMOTE CCCHANGE HOWL TWO
	Priority	14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_CostumeChange_InnerWill_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 1 45
	  PlayFX	Emotes\CostumeChanges\InnerWill\InnerWill.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_InnerWill_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_InnerWill_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 45
	  PlayFX	Emotes\CostumeChanges\InnerWill\InnerWill.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_InnerWill_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale		0.50

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_InnerWill
	Requires	FLY EMOTE CCCHANGE HOWL TWO
	Priority	25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_InnerWill_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 1 45
	  PlayFX	Emotes\CostumeChanges\InnerWill\InnerWill.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_InnerWill_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_C_flycycle
	NextMove	PreFall
	Flags		Cycle NOSIZESCALE
	Sets		CANTMOVE COSTUMECHANGE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 45
	  PlayFX	Emotes\CostumeChanges\InnerWill\InnerWill.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Costume Change - Light Magic
#############################################################

Move Emote_CostumeChange_LightMagic
	Requires	EMOTE CCCHANGE HOWL THREE
	Priority	14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_CostumeChange_LightMagic_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 1 45
	  PlayFX	Emotes\CostumeChanges\LightMagic\LightMagicChange.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_LightMagic_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_LightMagic_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 45
	  PlayFX	Emotes\CostumeChanges\LightMagic\LightMagicChange.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_LightMagic_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale		0.50

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_LightMagic
	Requires	FLY EMOTE CCCHANGE HOWL THREE
	Priority	25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_LightMagic_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 1 45
	  PlayFX	Emotes\CostumeChanges\LightMagic\LightMagicChange.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_LightMagic_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_C_flycycle
	NextMove	PreFall
	Flags		Cycle NOSIZESCALE
	Sets		CANTMOVE COSTUMECHANGE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 45
	  PlayFX	Emotes\CostumeChanges\LightMagic\LightMagicChange.fx 0 CONSTANT
	TEnd
MEnd

#############################################################
## Costume Change - Rainbow
#############################################################

Move Emote_CostumeChange_Rainbow
	Requires	EMOTE CCCHANGE HOWL FOUR
	Priority	14
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	Emote_CostumeChange_Rainbow_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 1 45
	  PlayFX	Emotes\CostumeChanges\Rainbow\RainbowChange.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Rainbow_Post
	Include sequencers\Members\Emotes_Idle.inc
	NextMove	Emote_CostumeChange_Rainbow_PostCycle
	Sets		CANTMOVE COSTUMECHANGE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\scream 45
	  PlayFX	Emotes\CostumeChanges\Rainbow\RainbowChange.fx 0 CONSTANT
	TEnd
MEnd

Move Emote_CostumeChange_Rainbow_PostCycle
	Include sequencers\Members\Emotes_Idle.inc
	Flags		Cycle
	Scale		0.50

	Type		male
	  Anim          male\comb_beatchest 95 144
	TEnd
MEnd

#############################################################

Move A_Emote_CostumeChange_Rainbow
	Requires	FLY EMOTE CCCHANGE HOWL FOUR
	Priority	25
	Interrupts	<DeathIRQ> <AttackIRQ> <IRQEmotesALL> <IRQEmotesCombat> <IRQEmotesRighthand> <IRQEmotesLefthand> <IRQEmotesDualhand> <IRQEmotesShield>
	NextMove	A_Emote_CostumeChange_Rainbow_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 1 45
	  PlayFX	Emotes\CostumeChanges\Rainbow\RainbowChange.fx 0 CONSTANT
	TEnd
MEnd

Move A_Emote_CostumeChange_Rainbow_Post
	Requires	FLY
	Include sequencers\Members\Emotes_Idle_Air.inc
	CycleMove	Fake_C_flycycle
	NextMove	PreFall
	Flags		Cycle NOSIZESCALE
	Sets		CANTMOVE COSTUMECHANGE
	Scale		0.625

	Type		male
	  Anim          male\air_scream 45
	  PlayFX	Emotes\CostumeChanges\Rainbow\RainbowChange.fx 0 CONSTANT
	TEnd
MEnd