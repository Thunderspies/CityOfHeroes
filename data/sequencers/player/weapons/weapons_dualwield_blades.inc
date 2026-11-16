#############################################################
## Weapons_DualWield.inc - Duel Wield Weapons
#############################################################
# Pre-attack (Draw Animation and 1st move of attack sequence, CANTMOVE flag in use)
#	Interrupts	<Ready> <AttackIRQ> <Combat> <CombatAttack> <CombatMove> <WepCombat> <WepAttack> <WepMove> <DualHandCombat> <DualHandAttack> <DualHandMove>
#	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ> <WepCombat> <WepAttack> <WepMove> <DualHandAttack> <DualHandMove> <MoveIRQ>
#
# Attack (next moves where CANTMOVE flag still in use)
#	Member		<DeathIRQ>
#
# Post Attack (moves after CANTMOVE flag)
#	Member		<DeathIRQ> <ReactIRQ> <AttackIRQ> <WepCombat> <WepAttack> <WepMove> <DualHandAttack> <DualHandMove> <MoveIRQ>

## DRAW ###########################################################

Move DualWield_Draw
	Requires	COMBAT WEAPON DUAL SPAWNBACK
	Priority        54
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_Draw_Post
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\DualBlade_Draw 1 31
	TEnd
MEnd

Move DualWield_Draw_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\DualBlade_Draw 32
	TEnd
MEnd

## BRAWL ###########################################################

#Move DualWield_Blades_KickBrawl_Pre
#	Requires	COMBAT WEAPON DUAL PUNCH BRAWL
#	Priority        5
#	Interrupts	<AttackIRQ> <ReactIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        DualWield_Blades_KickBrawl
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\Dual_Brawl 5 30
#	TEnd
#MEnd
#
#Move DualWield_Blades_KickBrawl
#	Requires	COMBAT
#	Priority        3
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <WepAttack> <AttackIRQ> <MoveIRQ> <IRQEmotesDualhand>
#	NextMove        DualWield_Blades_KickBrawl_Blend
#
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\Dual_Brawl 31 50
#	TEnd
#MEnd
#
#Move DualWield_Blades_KickBrawl_Blend
#	Requires	COMBAT
#	Priority        3
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <WepAttack> <DualHandCombat> <AttackIRQ> <PostMoveCycle> <MoveIRQ> <IRQEmotesDualhand>
#	NextMove        DualWield_Ready
#	Scale           0.50
#	Interpolate	15
#	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\dual_cycle 1 31
#	TEnd
#MEnd


## THROW BRAWL ###########################################################

Move DualWield_Blades_ThrowBrawl
	Requires	WEAPON COMBAT DUAL THROWBRAWL
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Blades_ThrowBrawl_Post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\dual_knife_throw 1 25
	  PitchAngle	0.5
	TEnd
MEnd

Move DualWield_Blades_ThrowBrawl_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		PitchToTarget ReqInputs

	Type		male
	  Anim          male\dual_knife_throw 26
	  PitchEnd	35
	  PitchAngle	0.5
	TEnd
MEnd


## ATTACK ###########################################################

Move DualWield_Slash_Draw
	Requires	COMBAT WEAPON DUAL ATTACK SLASH SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_Slash
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_Slash
	Requires	COMBAT WEAPON DUAL ATTACK SLASH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\wm_slash 3 30
	TEnd

	Type		huge
	  Anim          huge\wm_slash 3 30
	TEnd

	Type		fem
	  Anim          fem\wm_slash 3 30
	TEnd
MEnd

Move DualWield_Slash_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\wm_slash 31
	TEnd

	Type		huge
	  Anim          huge\wm_slash 31
	TEnd

	Type		fem
	  Anim          fem\wm_slash 31
	TEnd
MEnd


## LIGHT OPENING ###########################################################

Move DualWield_LightOpeningA_Draw
	Requires	COMBAT WEAPON DUAL ATTACK QUICK SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_LightOpeningA
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_LightOpeningA
	Requires	COMBAT WEAPON DUAL ATTACK QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_LightOpeningA_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_LightOpeningA 2 33
	TEnd
MEnd

Move DualWield_LightOpeningA_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_LightOpeningA 34
	TEnd
MEnd


## LIGHT OPENING B ###########################################################

Move DualWield_LightOpeningB_Draw
	Requires	COMBAT WEAPON DUAL ATTACK QUICK SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_LightOpeningB
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_LightOpeningB
	Requires	COMBAT WEAPON DUAL ATTACK QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_LightOpeningB_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_LightOpeningB 2 33
	TEnd
MEnd

Move DualWield_LightOpeningB_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_LightOpeningB 34
	TEnd
MEnd


## MODERATE OPENING ###########################################################

Move DualWield_ModerateOpening_Draw
	Requires	COMBAT WEAPON DUAL ATTACK MID ONE SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_ModerateOpening
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_ModerateOpening
	Requires	COMBAT WEAPON DUAL ATTACK MID ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_ModerateOpening_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_ModerateOpening 2 43
	TEnd
MEnd

Move DualWield_ModerateOpening_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_ModerateOpening 44
	TEnd
MEnd


## MODERATE BRIDGE ###########################################################

Move DualWield_ModerateBridge_Draw
	Requires	COMBAT WEAPON DUAL ATTACK MID TWO SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_ModerateBridge
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_ModerateBridge
	Requires	COMBAT WEAPON DUAL ATTACK MID TWO
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_ModerateBridge_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_ModerateBridge 2 33
	TEnd
MEnd

Move DualWield_ModerateBridge_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_ModerateBridge 34
	TEnd
MEnd


## AOE BRIDGE ###########################################################

Move DualWield_AOEBridge_Draw
	Requires	COMBAT WEAPON DUAL ATTACK SPIN SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_AOEBridge
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_AOEBridge
	Requires	COMBAT WEAPON DUAL ATTACK SPIN
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_AOEBridge_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_AOEBridge 2 70
	TEnd
MEnd

Move DualWield_AOEBridge_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_AOEBridge 71
	TEnd
MEnd


## FOLLOW UP ###########################################################

Move DualWield_FollowUp_Draw
	Requires	COMBAT WEAPON DUAL SPAWNBACK ATTACK STEAL
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_FollowUp
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_FollowUp
	Requires	COMBAT WEAPON DUAL ATTACK STEAL
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_FollowUp_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_FollowUp 2 38
	TEnd
MEnd

Move DualWield_FollowUp_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_FollowUp 39
	TEnd
MEnd


## TAUNT ###########################################################

Move DualWield_Taunt_Draw
	Requires	COMBAT WEAPON DUAL TAUNT VARIATIONA SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_Taunt
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_Taunt
	Requires	COMBAT WEAPON DUAL TAUNT VARIATIONA
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Taunt_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Taunt 2 60
	TEnd
MEnd

Move DualWield_Taunt_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Taunt 61
	TEnd
MEnd


## SPECIAL 1 ###########################################################

Move DualWield_Special1_Draw
	Requires	COMBAT WEAPON DUAL ATTACK STRONG SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_Special1
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_Special1
	Requires	COMBAT WEAPON DUAL ATTACK STRONG
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Special1_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_Special1 2 75
	TEnd
MEnd

Move DualWield_Special1_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_Special1 76
	TEnd
MEnd


## SPECIAL 2 ###########################################################

Move DualWield_Special2_Draw
	Requires	COMBAT WEAPON DUAL ATTACK CONE SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_Special2
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_Special2
	Requires	COMBAT WEAPON DUAL ATTACK CONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Special2_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_Special2 3 40
	TEnd
MEnd

Move DualWield_Special2_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_Special2 41
	TEnd
MEnd


## HIGH LOW ###########################################################

Move DualWield_HighLow_Draw
	Requires	COMBAT WEAPON DUAL ATTACK FIERCE SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_HighLow
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_HighLow
	Requires	COMBAT WEAPON DUAL ATTACK FIERCE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_HighLow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.75

	Type		male
	  Anim          male\DualBlade_Attack_HighLow 2 75
	TEnd
MEnd

Move DualWield_HighLow_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs
	Scale		0.75

	Type		male
	  Anim          male\DualBlade_Attack_HighLow 76
	TEnd
MEnd


## ASSASSISN'S STRIKE ###########################################################

Move DualWield_AssassinsStrike_Draw
	Requires	COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	DualWield_AssassinsStrike_Windup
	##Sets            CANTMOVE FLAG MUST NOT BE USED
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_AssassinsStrike_Windup
	Requires	COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB
	Priority        4
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove        DualWield_AssassinsStrike
	##Sets            CANTMOVE FLAG MUST NOT BE USED
	Flags		NOSIZESCALE
	Scale		1.6

	Type		male
	  Anim          male\DualBlade_Attack_AssassinsStrike 2 60
	TEnd
MEnd

Move DualWield_AssassinsStrike_Windup2
	Requires	COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove        DualWield_AssassinsStrike
	##Sets            CANTMOVE FLAG MUST NOT BE USED
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_AssassinsStrike 2 60
	TEnd
MEnd

#############################################################

Move DualWield_AssassinsStrike_Quick_Draw
	Requires	COMBAT WEAPON DUAL DUALBLADEBACKSTAB SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	DualWield_AssassinsStrike
	##Sets            CANTMOVE FLAG MUST NOT BE USED
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_AssassinsStrike
	Requires	COMBAT WEAPON DUAL ATTACK DUALBLADEBACKSTAB
	Priority        5
	Interrupts	<AttackIRQ> DualWield_AssassinsStrike_Draw DualWield_AssassinsStrike_Windup DualWield_AssassinsStrike_Windup2
	Member		<DeathIRQ>
	NextMove        DualWield_AssassinsStrike_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Attack_AssassinsStrike 61 91
	TEnd
MEnd

Move DualWield_AssassinsStrike_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\DualBlade_Attack_AssassinsStrike 92
	TEnd
MEnd

## PLACATE ###########################################################

Move DualWield_Placate_Draw
	Requires	COMBAT WEAPON DUAL PLACATE SPAWNBACK
	Priority	55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_Placate
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_Placate
	Requires	COMBAT WEAPON DUAL PLACATE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Placate_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\DualBlade_Placate 2 45
	TEnd
MEnd

Move DualWield_Placate_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\DualBlade_Placate 46
	TEnd
MEnd


## GOBLIN - DRAW DUAL DAGGERS ###########################################################

Move Draw_Daggers
	Requires	COMBAT WEAPON SPAWNSIDE DUAL KNIFE
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <Ready> <AttackIRQ> ##<PreMove> ## <Combat>
	NextMove        Draw_Daggers_2
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\goblin_draw_dualdaggers 1 30
	TEnd
MEnd

Move Draw_Daggers_2
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Ready

	Type		male
	  Anim          male\goblin_draw_dualdaggers 31
	TEnd
MEnd


## GOBLIN - DAGGER SLASH ###########################################################

Move Daggers_Slash_A_Draw
	Requires	COMBAT WEAPON DUAL KNIFE SLASH SPAWNSIDE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Daggers_Slash_A
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\goblin_draw_dualdaggers 1 30
	TEnd
MEnd

Move Daggers_Slash_A
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Daggers_Slash_A_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\goblin_sword_slash 1 20  ###goblin_daggerslash_L 1 20
	  PlayFX	Generic\SFX_stab.fx 15
	TEnd
MEnd

Move Daggers_Slash_A_Post
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Fake_DualReady

	Type		male
	  Anim          male\goblin_sword_slash 21  ###goblin_daggerslash_L 21
	TEnd
MEnd

#############################################################

Move Daggers_Slash_B
	Requires	COMBAT WEAPON DUAL KNIFE SLASH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Daggers_Slash_B_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\goblin_daggerstabbie 1 20
	  PlayFX	Generic\SFX_stab.fx 11
	  PlayFX	Generic\SFX_stab.fx 15
	TEnd
MEnd

Move Daggers_Slash_B_post
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Fake_DualReady

	Type		male
	  Anim          male\goblin_daggerstabbie 21
	TEnd
MEnd

#############################################################

Move Daggers_Slash_C
	Requires	COMBAT WEAPON DUAL KNIFE SLASH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Daggers_Slash_A_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\goblin_sword_slash 1 20  ###goblin_daggerslash_L 1 20
	  PlayFX	Generic\SFX_stab.fx 15
	TEnd
MEnd