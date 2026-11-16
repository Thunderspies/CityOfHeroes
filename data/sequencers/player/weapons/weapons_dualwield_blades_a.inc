#############################################################
## Weapons_DualWield.inc - Duel Wield Weapons
#############################################################

## AIR DRAW WEAPON ###########################################################

Move A_DualWield_Draw
	Requires	FLY COMBAT WEAPON DUAL SPAWNBACK
	Priority	65
	Interrupts	<Ready> <AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_DualWield_Draw_Post
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Draw 5 36
	TEnd
MEnd

Move A_DualWield_Draw_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Draw 37
	TEnd
MEnd

## AIR BRAWL ###########################################################

#Move A_DualWield_Blades_KickBrawl_Pre
#	Requires	FLY COMBAT WEAPON DUAL PUNCH BRAWL
#	Priority	16
#	Interrupts	<AttackIRQ> <ReactIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_DualWield_Blades_KickBrawl
#	Sets            CANTMOVE
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_dual_brawl 5 30
#	TEnd
#MEnd
#
#Move A_DualWield_Blades_KickBrawl
#	Requires	FLY COMBAT
#	Priority	15
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
#	NextMove        A_DualWield_Blades_KickBrawl_Blend
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_dual_brawl 31 50
#	TEnd
#MEnd
#
#Move A_DualWield_Blades_KickBrawl_Blend
#	Requires	FLY COMBAT
#	Priority	14
#	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ> <FlyWepAttack>
#	NextMove        DualWield_FlyCycle
#	Scale           0.50
#	Interpolate	15
#	Flags		NOSIZESCALE
#
#	Type		male
#	  Anim          male\air_dual_cycle 1 31
#	TEnd
#MEnd


## AIR LIGHT OPENING A ###########################################################

Move A_DualWield_LightOpeningA_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK QUICK SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_LightOpeningA
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_LightOpeningA
	Requires	FLY COMBAT WEAPON DUAL ATTACK QUICK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_LightOpeningA_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_LightOpeningA 2 33
	TEnd
MEnd

Move A_DualWield_LightOpeningA_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_LightOpeningA 34
	TEnd
MEnd


## AIR LIGHT OPENING B ###########################################################

Move A_DualWield_LightOpeningB_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK QUICK SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_LightOpeningB
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_LightOpeningB
	Requires	FLY COMBAT WEAPON DUAL ATTACK QUICK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_LightOpeningB_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_LightOpeningB 2 33
	TEnd
MEnd

Move A_DualWield_LightOpeningB_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_LightOpeningB 34
	TEnd
MEnd


## AIR MODERATE OPENING ###########################################################

Move A_DualWield_ModerateOpening_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK MID ONE SPAWNBACK
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_ModerateOpening
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_ModerateOpening
	Requires	FLY COMBAT WEAPON DUAL ATTACK MID ONE
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_ModerateOpening_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_ModerateOpening 2 43
	TEnd
MEnd

Move A_DualWield_ModerateOpening_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_ModerateOpening 44
	TEnd
MEnd


## AIR MODERATE BRIDGE ###########################################################

Move A_DualWield_ModerateBridge_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK MID TWO SPAWNBACK
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_ModerateBridge
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_ModerateBridge
	Requires	FLY COMBAT WEAPON DUAL ATTACK MID TWO
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_ModerateBridge_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_ModerateBridge 2 33
	TEnd
MEnd

Move A_DualWield_ModerateBridge_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_ModerateBridge 34
	TEnd
MEnd


## AIR AOE BRIDGE ###########################################################

Move A_DualWield_AOEBridge_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK SPIN SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_AOEBridge
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_AOEBridge
	Requires	FLY COMBAT WEAPON DUAL ATTACK SPIN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_AOEBridge_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_AOEBridge 2 70
	TEnd
MEnd

Move A_DualWield_AOEBridge_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_AOEBridge 71
	TEnd
MEnd


## AIR FOLLOW UP ###########################################################

Move A_DualWield_FollowUp_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK STEAL SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_FollowUp
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_FollowUp
	Requires	FLY COMBAT WEAPON DUAL ATTACK STEAL
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_FollowUp_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_FollowUp 2 38
	TEnd
MEnd

Move A_DualWield_FollowUp_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_FollowUp 39
	TEnd
MEnd


## AIR TAUNT ###########################################################

Move A_DualWield_Taunt_Draw
	Requires	FLY COMBAT WEAPON DUAL TAUNT VARIATIONA SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_Taunt
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_Taunt
	Requires	FLY COMBAT WEAPON DUAL TAUNT VARIATIONA
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_Taunt_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Taunt 2 60
	TEnd
MEnd

Move A_DualWield_Taunt_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Taunt 60
	TEnd
MEnd


## AIR SPECIAL 1 ###########################################################

Move A_DualWield_Special1_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK STRONG SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_Special1
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_Special1
	Requires	FLY COMBAT WEAPON DUAL ATTACK STRONG
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_Special1_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_Special1 2 75
	TEnd
MEnd

Move A_DualWield_Special1_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_Special1 75
	TEnd
MEnd


## AIR SPECIAL 2 ###########################################################

Move A_DualWield_Special2_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK CONE SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_Special2
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_Special2
	Requires	FLY COMBAT WEAPON DUAL ATTACK CONE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_Special2_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_Special2 3 40
	TEnd
MEnd

Move A_DualWield_Special2_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_Special2 41
	TEnd
MEnd


## AIR HIGH LOW ###########################################################

Move A_DualWield_HighLow_Draw
	Requires	FLY COMBAT WEAPON DUAL ATTACK FIERCE SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_HighLow
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_HighLow
	Requires	FLY COMBAT WEAPON DUAL ATTACK FIERCE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_HighLow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.75

	Type		male
	  Anim          male\air_DualBlade_Attack_HighLow 2 75
	TEnd
MEnd

Move A_DualWield_HighLow_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs
	Scale		0.75

	Type		male
	  Anim          male\air_DualBlade_Attack_HighLow 76
	TEnd
MEnd


## AIR ASSASSISN'S STRIKE ###########################################################

Move A_DualWield_AssassinsStrike_Draw
	Requires	FLY COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	A_DualWield_AssassinsStrike_Windup
	##Sets            CANTMOVE FLAG MUST NOT BE USED
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_AssassinsStrike_Windup
	Requires	FLY COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB
	Priority	15
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove        A_DualWield_AssassinsStrike
	Flags		NOSIZESCALE
	##Sets            CANTMOVE FLAG MUST NOT BE USED
	Scale		1.6

	Type		male
	  Anim          male\air_DualBlade_Attack_AssassinsStrike 2 60
	TEnd
MEnd

Move A_DualWield_AssassinsStrike_Windup2
	Requires	FLY COMBAT WEAPON DUAL WINDUPDUALBLADEBACKSTAB
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove        A_DualWield_AssassinsStrike
	Flags		NOSIZESCALE
	##Sets            CANTMOVE FLAG MUST NOT BE USED

	Type		male
	  Anim          male\air_DualBlade_Attack_AssassinsStrike 2 60
	TEnd
MEnd

#############################################################

Move A_DualWield_AssassinsStrike_Quick_Draw
	Requires	FLY COMBAT WEAPON DUAL DUALBLADEBACKSTAB SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ> <IRQEmotesDualhand>
	NextMove	A_DualWield_AssassinsStrike
	##Sets            CANTMOVE FLAG MUST NOT BE USED
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_AssassinsStrike
	Requires	FLY COMBAT WEAPON DUAL ATTACK DUALBLADEBACKSTAB
	Priority	16
	Interrupts	<AttackIRQ> A_DualWield_AssassinsStrike_Windup A_DualWield_AssassinsStrike_Windup2
	Member		<DeathIRQ>
	NextMove        A_DualWield_AssassinsStrike_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Attack_AssassinsStrike 61 91
	TEnd
MEnd

Move A_DualWield_AssassinsStrike_Post
	Requires	FLY
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Attack_AssassinsStrike 92
	TEnd
MEnd


## AIR PLACATE ###########################################################

Move A_DualWield_Placate_Draw
	Requires	FLY COMBAT WEAPON DUAL PLACATE SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_DualWield_Placate
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim		male\air_DualBlade_Draw 5 31
	TEnd
MEnd

Move A_DualWield_Placate
	Requires	FLY COMBAT WEAPON DUAL PLACATE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_Placate_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_DualBlade_Placate 2 45
	TEnd
MEnd

Move A_DualWield_Placate_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        DualWield_FlyCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_DualBlade_Placate 46
	TEnd
MEnd