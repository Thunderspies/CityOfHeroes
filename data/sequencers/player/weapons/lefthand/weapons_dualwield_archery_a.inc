#############################################################
## Weapons_DualWield_Archery.inc - Archery (Flying)
#############################################################

## WEAPON DRAW ###########################################################

Move A_Draw_Bow
	Requires	FLY COMBAT WEAPON DUAL ARCHERY SPAWNHIP
	Priority	66
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd


## READY ###########################################################

Move A_Bow_Ready
	Requires	FLY COMBAT WEAPON DUAL ARCHERY
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	Flags		Cycle
	NextMove        DualWield_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd


## HALF READY (transitions into full cycle) ###########################################################

Move A_Bow_half2
	Requires	FLY COMBAT WEAPON DUAL ARCHERY
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Scale           0.50

	Type		male
	  Anim          male\air_dual_cycle 15 31
	TEnd
MEnd


## ARCHERY KICK BRAWL ###########################################################

Move A_Archery_KickBrawl
	Requires	FLY COMBAT WEAPON DUAL REV PUNCH ARCHERY BRAWL
	Priority	18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Archery_KickBrawl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_dual_brawl 5 30
	TEnd
MEnd

Move A_Archery_KickBrawl_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Archery_KickBrawl_PostBlend
	Flags		ReqInputs

	Type		male
	  Anim          male\air_dual_brawl 31 50
	TEnd
MEnd

Move A_Archery_KickBrawl_PostBlend
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		ReqInputs
	Interpolate	15
	Scale           0.50

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd


## AIR ARACHERY BOOSTUP ###########################################################

Move A_Archery_Boostup
	Requires	FLY COMBAT WEAPON DUAL ARCHERY BOOSTUP
	Priority	16
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Archery_Boostup_Post
	Flags		FixArmReg

	Type		male
	  Anim          male\air_boostup 1
	TEnd

	Type		huge
	  Anim          huge\air_boostup 1
	TEnd

	Type		fem
	  Anim          fem\air_boostup 1
	TEnd
MEnd

Move A_Archery_Boostup_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove	A_Bow_Ready
	Interpolate	15
	Flags		FixArmReg ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd


## ATTACK - QUICK ###########################################################

Move A_Bow_Quick_Draw
	Requires	FLY COMBAT WEAPON DUAL ARCHERY QUICK SPAWNHIP
	Priority	67
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Quick
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd

Move A_Bow_Quick
	Requires	FLY COMBAT WEAPON DUAL ARCHERY QUICK
	Priority	16
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Quick_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWQUICK

	Type		male
	  Anim          male\air_bow_quickshoot 1 31
	  PlayFX	WEAPONS\Bow\bowstring_quick.fx 0
	  PitchStart	2
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Bow_Quick_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\air_bow_quickshoot 32
	TEnd
MEnd


## ATTACK - MID ###########################################################

Move A_Bow_Mid_Draw
	Requires	FLY COMBAT WEAPON DUAL ARCHERY MID SPAWNHIP
	Priority	67
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Mid
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd

Move A_Bow_Mid
	Requires	FLY COMBAT WEAPON DUAL ARCHERY MID
	Priority	17
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Mid_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWMID

	Type		male
	  Anim          male\air_bow_midshoot 18 48
	  PlayFX	WEAPONS\Bow\bowstring_mid.fx 0
	  PitchStart	2
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Bow_Mid_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_bow_midshoot 49
	  PitchEnd	74
	  PitchAngle	0.5
	TEnd
MEnd


## ATTACK - HEAVY ###########################################################

Move A_Bow_Heavy_Draw
	Requires	FLY COMBAT WEAPON DUAL ARCHERY ATTACK STRONG SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Heavy
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd

Move A_Bow_Heavy
	Requires	FLY COMBAT WEAPON DUAL ARCHERY ATTACK STRONG
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Heavy_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWHEAVY

	Type		male
	  Anim          male\air_bow_snipershoot 15 68
	   PlayFX	WEAPONS\Bow\bowstring_sniper.fx 0
	   PitchStart	22
	  PitchAngle	0.7
	TEnd
MEnd

Move A_Bow_Heavy_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_bow_snipershoot 69
	  PitchEnd	93
	  PitchAngle	0.7
	TEnd
MEnd


## ATTACK - CONE ###########################################################

Move A_Bow_Cone_Draw
	Requires	FLY COMBAT WEAPON DUAL ARCHERY SPAWNHIP CONE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Cone
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd

Move A_Bow_Cone
	Requires	FLY COMBAT WEAPON DUAL ARCHERY CONE
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Cone_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWCONE

	Type		male
	  Anim          male\air_bow_coneshoot 15 50
	  PlayFX	WEAPONS\Bow\bowstring_cone.fx 0
	  PitchStart	2
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Bow_Cone_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_bow_coneshoot 51
	  PitchEnd	72
	  PitchAngle	0.5
	TEnd
MEnd


## ATTACK - ARC ###########################################################

Move A_Bow_Arc_Draw	## (don't add pitchtotarget to this power!) ##
	Requires	FLY COMBAT WEAPON DUAL ARCHERY LAUNCH SPAWNHIP
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Arc
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd
MEnd

Move A_Bow_Arc
	Requires	FLY COMBAT WEAPON DUAL ARCHERY LAUNCH
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Bow_Arc_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	SticksOnChild   BOWARCH

	Type		male
	  Anim          male\air_bow_arc 1 120
	  PlayFX	WEAPONS\Bow\bowstring_arc.fx 0
	TEnd
MEnd

Move A_Bow_Arc_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_bow_arc 121
	TEnd
MEnd


## ATTACK - SNIPER ###########################################################

Move A_Bow_SniperWindup_Draw
	Requires	FLY COMBAT WEAPON DUAL ARCHERY WINDUP SPAWNHIP
	Priority	67
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_BowSniperWindup_Cycle
	Flags		PitchToTarget NOSIZESCALE
	#Sets		CANTMOVE ## Don't use CANTMOVE on interruptible moves
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY WINDUP SPAWNHIP

	Type		male
	  Anim          male\air_bow_snipershoot_windup_1st 1 69
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_pre_1st.fx 0 CONSTANT
	  PlayFX	WEAPONS\BOW\Archery\arrow_windupsniper_cycle.fx 20 CONSTANT
	  PitchStart	42
	  PitchAngle	0.7
	TEnd
MEnd

Move A_Bow_SniperWindup_Pre
	Requires	FLY COMBAT WEAPON DUAL ARCHERY WINDUP
	Priority	16
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_BowSniperWindup_Cycle
	Flags		PitchToTarget NOSIZESCALE
	#Sets		CANTMOVE ## Don't use CANTMOVE on interruptible moves
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY WINDUP

	Type		male
	  Anim          male\air_bow_snipershoot_windup 1 49
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_pre.fx 0 CONSTANT
	  PlayFX	WEAPONS\BOW\Archery\arrow_windupsniper_cycle.fx 0 CONSTANT
	  PitchStart	22
	  PitchAngle	0.7
	TEnd
MEnd

Move A_BowSniperWindup_Cycle
	Requires	FLY COMBAT WEAPON DUAL ARCHERY WINDUP
	Priority	16
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle PitchToTarget NOSIZESCALE
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY WINDUP
	NextMove        A_BowSniper_Shoot

	Type		male
	  Anim          male\air_bow_snipershoot_windup 50 60
	  PlayFX	WEAPONS\BOW\bowstring_drawn.fx 0 CONSTANT
	  PlayFX	WEAPONS\BOW\Archery\arrow_windupsniper_cycle.fx 0 CONSTANT
	  PitchAngle	0.7
	TEnd
MEnd

#############################################################

Move A_BowSniper_Shoot
	Requires	FLY COMBAT WEAPON DUAL ARCHERY SHOOT
	Priority	16
	Interrupts	<AttackIRQ> A_Bow_SniperWindup_Pre A_BowSniperWindup_Cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_BowSniper_Shoot_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY SHOOT LEFTHAND

	Type		male
	  Anim          male\air_bow_snipershoot_windup 61 75
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_post.fx 0 CONSTANT
	  PitchAngle	0.7
	TEnd
MEnd

Move A_BowSniper_Shoot_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\air_bow_snipershoot_windup 76
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_post.fx 0 CONSTANT
	  PitchEnd	100
	  PitchAngle	0.7
	TEnd
MEnd