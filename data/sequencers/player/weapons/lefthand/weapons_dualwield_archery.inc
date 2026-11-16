#############################################################
## Weapons_DualWield_Archery.inc - Archery
#############################################################

## ARCHERY KICK BRAWL ###########################################################

Move Archery_KickBrawl
	Requires	COMBAT WEAPON DUAL REV PUNCH ARCHERY BRAWL
	Priority        7
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Archery_KickBrawl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\doorsmash_kick 5 30
	TEnd
MEnd

Move Archery_KickBrawl_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Archery_KickBrawl_PostBlend
	Flags		ReqInputs

	Type		male
	  Anim          male\doorsmash_kick 31 50
	TEnd
MEnd

Move Archery_KickBrawl_PostBlend
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Interpolate	15
	Flags		ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

## WEAPON DRAW ###########################################################

Move Draw_Bow
	Requires	COMBAT WEAPON DUAL ARCHERY SPAWNHIP
	Priority        55
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

## READY TO BOW READY ###########################################################

#Move Ready_To_BowReady
#	Requires	COMBAT WEAPON DUAL REV ARCHERY
#	Priority        5
#	Interrupts	<Ready> <Combat> <WepCombat>
#	Include sequencers\Members\Combat_Weapon_Lefthand.inc
#	NextMove        Bow_Ready
#	Flags		NOSIZESCALE
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim          male\ninjasword_draw 1 20
#	TEnd
#MEnd

## READY ###########################################################

Move Bow_Ready
	Requires	COMBAT WEAPON DUAL ARCHERY
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	Flags		Cycle ###FixArmReg
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

## READY (Half Speed) ###########################################################

Move Bow_half2
	Requires	COMBAT WEAPON DUAL ARCHERY
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		NOSIZESCALE ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\dual_cycle 15 31
	TEnd
MEnd

## BOOSTUP ###########################################################

Move Archery_Boostup
	Requires	COMBAT WEAPON DUAL REV ARCHERY BOOSTUP
	Priority        6
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Archery_Boostup_Post

	Type		male
	  Anim          male\absorb 1
	TEnd

	Type		huge
	  Anim          huge\boostup 1
	TEnd

	Type		fem
	  Anim          fem\boostup 1
	TEnd
MEnd

Move Archery_Boostup_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove	Bow_Ready
	Interpolate	15
	Flags		FixArmReg ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

## ATTACK - QUICK ###########################################################

Move Bow_Quick_Draw
	Requires	COMBAT WEAPON DUAL REV ARCHERY QUICK SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Quick
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Bow_Quick
	Requires	COMBAT WEAPON DUAL REV ARCHERY QUICK
	Priority        6
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Quick_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWQUICK

	Type		male
	  Anim          male\bow_quickshoot 1 31
	  PlayFX	WEAPONS\BOW\bowstring_quick.fx 0
	  PitchStart	2
	  PitchAngle	0.5
	TEnd
MEnd

Move Bow_Quick_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\bow_quickshoot 32
	TEnd
MEnd


## ATTACK - MID ###########################################################

Move Bow_Mid_Draw
	Requires	COMBAT WEAPON DUAL REV ARCHERY MID SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Mid
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Bow_Mid
	Requires	COMBAT WEAPON DUAL REV ARCHERY MID
	Priority        6
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Mid_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWMID

	Type		male
	  Anim          male\bow_midshoot 18 48
	  PlayFX	WEAPONS\BOW\bowstring_Mid.fx 0
	  PitchStart	2
	  PitchAngle	0.5
	TEnd
MEnd

Move Bow_Mid_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\bow_midshoot 49
	  PitchEnd	74
	  PitchAngle	0.5
	TEnd
MEnd


## ATTACK - HEAVY ###########################################################

Move Bow_Heavy_Draw
	Requires	COMBAT WEAPON DUAL ARCHERY ATTACK STRONG SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        Bow_Heavy
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Bow_Heavy
	Requires	COMBAT WEAPON DUAL ARCHERY ATTACK STRONG
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        Bow_Heavy_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWHEAVY

	Type		male
	  Anim          male\bow_snipershoot 15 68
	   PlayFX	WEAPONS\BOW\bowstring_sniper.fx 0
	   PitchStart	22
	   PitchAngle	0.7
	TEnd
MEnd

Move Bow_Heavy_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\bow_snipershoot 69
	  PitchEnd	93
	   PitchAngle	0.7
	TEnd
MEnd


## ATTACK - CONE ###########################################################

Move Bow_Cone_Draw
	Requires	COMBAT WEAPON DUAL ARCHERY SPAWNHIP CONE
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Cone
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Bow_Cone
	Requires	COMBAT WEAPON DUAL ARCHERY CONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Cone_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild   BOWCONE

	Type		male
	  Anim          male\bow_coneshoot 15 50
	  PlayFX	WEAPONS\BOW\bowstring_cone.fx 0
	  PitchStart	2
	  PitchAngle	0.5
	TEnd
MEnd

Move Bow_Cone_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\bow_coneshoot 51
	  PitchEnd	72
	  PitchAngle	0.5
	TEnd
MEnd


## ATTACK - ARC ###########################################################

Move Bow_Arc_Draw	## (don't add pitchtotarget to this power!) ##
	Requires	COMBAT WEAPON DUAL ARCHERY LAUNCH SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Arc
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Bow_Arc
	Requires	COMBAT WEAPON DUAL ARCHERY LAUNCH
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Bow_Arc_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	SticksOnChild   BOWARCH

	Type		male
	  Anim          male\bow_arc 1 120
	  PlayFX	WEAPONS\BOW\bowstring_arc.fx 0
	TEnd
MEnd

Move Bow_Arc_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\bow_arc 121
	TEnd
MEnd


## ATTACK - SNIPER ###########################################################

Move Bow_SniperWindup_Draw
	Requires	COMBAT WEAPON DUAL ARCHERY WINDUP SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        BowSniperWindup_Cycle
	Flags		PitchToTarget NOSIZESCALE
	#Sets		CANTMOVE ## Don't use CANTMOVE on interruptible moves
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY WINDUP SPAWNHIP

	Type		male
	  Anim          male\bow_snipershoot_windup_1st 1 69
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_pre_1st.fx 0 CONSTANT
	  PlayFX	WEAPONS\BOW\Archery\arrow_windupsniper_cycle.fx 20 CONSTANT
	  PitchStart	42
	  PitchAngle	0.7
	TEnd
MEnd

Move Bow_SniperWindup_Pre
	Requires	COMBAT WEAPON DUAL ARCHERY WINDUP
	Priority        5
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        BowSniperWindup_Cycle
	Flags		PitchToTarget NOSIZESCALE
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY WINDUP

	Type		male
	  Anim          male\bow_snipershoot_windup 1 49
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_pre.fx 0 CONSTANT
	  PlayFX	WEAPONS\BOW\Archery\arrow_windupsniper_cycle.fx 0 CONSTANT
	  PitchStart	22
	  PitchAngle	0.7
	TEnd
MEnd

Move BowSniperWindup_Cycle
	Requires	COMBAT WEAPON DUAL ARCHERY WINDUP
	Priority        5
	Include sequencers\Members\Windups.inc
	Flags		Cycle PitchToTarget NOSIZESCALE
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY WINDUP
	NextMove        BowSniper_Shoot

	Type		male
	  Anim          male\bow_snipershoot_windup 50 60
	  PlayFX	WEAPONS\BOW\bowstring_drawn.fx 0 CONSTANT
	  PlayFX	WEAPONS\BOW\Archery\arrow_windupsniper_cycle.fx 0 CONSTANT
	  PitchAngle	0.7
	TEnd
MEnd

#############################################################

Move BowSniper_Shoot
	Requires	COMBAT WEAPON DUAL ARCHERY SHOOT
	Priority        5
	Interrupts	<AttackIRQ> Bow_SniperWindup_Pre BowSniperWindup_Cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        BowSniper_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild	COMBAT WEAPON DUAL ARCHERY SHOOT

	Type		male
	  Anim          male\bow_snipershoot_windup 61 75
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_post.fx 0 CONSTANT
	  PitchAngle	0.7
	TEnd
MEnd

Move BowSniper_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Bow_Ready
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\bow_snipershoot_windup 76
	  PlayFX	WEAPONS\BOW\bowstring_windupsniper_post.fx 0 CONSTANT
	  PitchEnd	100
	  PitchAngle	0.7
	TEnd
MEnd