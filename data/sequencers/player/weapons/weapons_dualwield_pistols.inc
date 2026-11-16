#############################################################
## Weapons_DualWield_Pisotls.inc - Dual Wield Pistols
#############################################################

## DRAW DUAL PISTOLS ###########################################################

Move Draw_PISTOLS
	Requires	COMBAT WEAPON DUAL PISTOLS SPAWNHIP
	Priority        55
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Draw_PISTOLS_post
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move Draw_PISTOLS_post
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready

	Type		male
	  Anim          male\draw_dual_guns 31
	TEnd
MEnd

#############################################################

Move A_Draw_PISTOLS
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SPAWNHIP
	Priority        66
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_Draw_PISTOLS_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_Draw_PISTOLS_post
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 31
	TEnd
MEnd

## DUAL PISTOLS READY ###########################################################

Move Pistols_Ready_NoInterpolate
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Scale           0.75
	Interpolate	0
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

Move PISTOLS_Ready
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd

#############################################################

Move A_PISTOLS_Ready_Blend
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Flags		Cycle NOSIZESCALE ReqInputs
	CycleMove	A_PISTOLS_Ready
	NextMove        DualWield_PreFall
	Scale           0.75
	Interpolate	15

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd

Move A_PISTOLS_Ready
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	Flags		Cycle
	#NextMove        DualWield_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_dual_cycle 1 31
	TEnd
MEnd

## BRAWL ###########################################################

Move DualWield_Pistols_PistolWhipR_Pre
	Requires	COMBAT WEAPON DUAL PISTOLS PUNCH BRAWL
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Pistols_PistolWhipR
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_PistolWhipR 1 20
	TEnd
MEnd

Move DualWield_Pistols_PistolWhipR
	Requires	COMBAT WEAPON DUAL PISTOLS
	Priority        4
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_PistolWhipR 21
	TEnd
MEnd

#############################################################

Move DualWield_Pistols_PistolWhipL_Pre
	Requires	COMBAT WEAPON DUAL PISTOLS PUNCH BRAWL
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_Pistols_PistolWhip2L
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_PistolWhipL 1 20
	TEnd
MEnd

Move DualWield_Pistols_PistolWhip2L
	Requires	COMBAT WEAPON DUAL PISTOLS
	Priority        4
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_PistolWhipL 21
	TEnd
MEnd


## AIR BRAWL ###########################################################

Move A_DualWield_Pistols_PistolWhipR_Pre
	Requires	FLY COMBAT WEAPON DUAL PISTOLS PUNCH BRAWL
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_Pistols_PistolWhipR
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_Air_PistolWhipR 1 20
	TEnd
MEnd

Move A_DualWield_Pistols_PistolWhipR
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Thug_Air_PistolWhipR 21
	TEnd
MEnd

#############################################################

Move A_DualWield_Pistols_PistolWhipL_Pre
	Requires	FLY COMBAT WEAPON DUAL PISTOLS PUNCH BRAWL
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualWield_Pistols_PistolWhipL
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_Air_PistolWhipL 1 20
	TEnd
MEnd

Move A_DualWield_Pistols_PistolWhipL
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        a_PISTOLS_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\Thug_Air_PistolWhipL 21
	TEnd
MEnd

###############################
####### PISTOLS single shot  ######
###############################

Move Pistols_Single_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS BIGSINGLESHOT SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistols_Single_Left
	Flags		CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move Pistols_Single_Draw_Gunslinger
	Requires	COMBAT WEAPON DUAL PISTOLS BIGSINGLESHOT SPAWNHIP GUNSLINGER
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistols_Single_Left
	Flags		CANTMOVE

	Type		male
	  Anim          male\dual_gunslinger_draw 1 30
	TEnd
MEnd

#############################################################

Move Pistols_Single_Left
	Requires	COMBAT WEAPON DUAL PISTOLS BIGSINGLESHOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistols_Single_Left_Post
	Flags		CANTMOVE PitchToTarget
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\dual_guns_single_shot 1 35
	  PitchAngle	0.3
	TEnd
MEnd

Move Pistols_Single_Left_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		PitchToTarget

	Type		male
	  Anim          male\dual_guns_single_shot 36
	  PitchAngle	0.3
	  PitchEnd	51
	TEnd
MEnd

#############################################################

Move Pistols_Single_Right
	Requires	COMBAT WEAPON DUAL PISTOLS BIGSINGLESHOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Pistols_Single_Right_Post
	Flags		CANTMOVE PitchToTarget
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\dual_guns_Altsingle_shot 1 35
	  PitchAngle	-0.1
	TEnd
MEnd

Move Pistols_Single_Right_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		PitchToTarget

	Type		male
	  Anim          male\dual_guns_Altsingle_shot 36
	  PitchAngle	-0.1
	  PitchEnd	45
	TEnd
MEnd

#############################################################

Move A_Pistols_Single_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BIGSINGLESHOT SPAWNHIP
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Pistols_Single_Right
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

#############################################################

Move A_Pistols_Single_Right
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BIGSINGLESHOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Pistols_Single_Right_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE PitchToTarget
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\air_dual_guns_Altsingle_shot 1 35
	  PitchAngle	-0.1
	TEnd
MEnd

Move A_Pistols_Single_Right_Post
	Requires	FLY
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_dual_guns_Altsingle_shot 36
	  PitchAngle	-0.1
	  PitchEnd	45
	TEnd
MEnd

#############################################################

Move A_Pistols_Single_Left
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BIGSINGLESHOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Pistols_Single_Left_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE PitchToTarget
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\air_dual_guns_single_shot 1 35
	  PitchAngle	0.3
	TEnd
MEnd

Move A_Pistols_Single_Left_Post
	Requires	FLY
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_dual_guns_single_shot 36
	  PitchAngle	0.3
	  PitchEnd	51
	TEnd
MEnd

###############################
####### PISTOLS Big shot  ######
###############################

Move PISTOLS_Bigshot_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS BIGSHOT SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_Bigshot
	Sets		CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move GunslingerDual_Bigshot_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS BIGSHOT SPAWNHIP GUNSLINGER
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_Bigshot
	Sets            CANTMOVE

	Type		male
	  Anim          male\dual_gunslinger_draw 1 30
	TEnd
MEnd

#############################################################

Move PISTOLS_Bigshot
	Requires	COMBAT WEAPON DUAL PISTOLS BIGSHOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_Bigshot_Post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\dual_guns_big_shot 1 35
	  PitchAngle	0.2
	TEnd
MEnd

Move PISTOLS_Bigshot_Post
	Requires	COMBAT WEAPON DUAL PISTOLS
	Priority        4
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		PitchToTarget

	Type		male
	  Anim          male\dual_guns_big_shot 36
	  PitchEnd	45
	  PitchAngle	0.2
	TEnd
MEnd

#############################################################

Move A_PISTOLS_Bigshot_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BIGSHOT SPAWNHIP
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_PISTOLS_Bigshot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_PISTOLS_Bigshot
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BIGSHOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_PISTOLS_Bigshot_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_dual_guns_big_shot 1 35
	  PitchAngle	0.2
	TEnd
MEnd

Move A_PISTOLS_Bigshot_Post
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_dual_guns_big_shot 36
	  PitchEnd	45
	  PitchAngle	0.2
	TEnd
MEnd


###############################
####### PISTOLS 6 shot  ######
###############################

Move PISTOLS_6Shot_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS BIGMULTISHOT SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_6Shot
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

#############################################################

Move GunslingerDual_6shot_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS BIGMULTISHOT SPAWNHIP GUNSLINGER
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_6Shot
	Sets            CANTMOVE

	Type		male
	  Anim          male\dual_gunslinger_draw 1 30
	TEnd
MEnd

#############################################################

Move PISTOLS_6Shot
	Requires	COMBAT WEAPON DUAL PISTOLS BIGMULTISHOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_6Shot_Post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\dual_guns_6_shot 1 55
	  PitchAngle	0.2
	TEnd
MEnd

Move PISTOLS_6Shot_Post
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		PitchToTarget

	Type		male
	  Anim          male\dual_guns_6_shot 56
	  PitchAngle	0.2
	  PitchEnd	55
	TEnd
MEnd

#############################################################

Move A_PISTOLS_6Shot_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BIGMULTISHOT SPAWNHIP
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_PISTOLS_6Shot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_PISTOLS_6Shot
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BIGMULTISHOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_PISTOLS_6Shot_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PitchToTarget

	Type		male
	  Anim          male\air_dual_guns_6_shot 1 55
	  PitchAngle	0.2
	TEnd
MEnd

Move A_PISTOLS_6Shot_Post
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_dual_guns_6_shot 56
	  PitchAngle	0.2
	  PitchEnd	55
	TEnd
MEnd


###############################
####### PISTOLS cone  ######
###############################

Move PISTOLS_Cone_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS CONE SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_Cone
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

#############################################################

Move GunslingerDual_CONE_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS CONE GUNSLINGER SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_Cone
	Sets            CANTMOVE

	Type		male
	  Anim          male\dual_gunslinger_draw 1 30
	TEnd
MEnd

#############################################################

Move PISTOLS_Cone
	Requires	COMBAT WEAPON DUAL PISTOLS CONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_Cone_Post
	Flags		CANTMOVE PitchToTarget

	Type		male
	  Anim          male\dual_guns_cone 1 85
	  PitchAngle	0.2
	TEnd
MEnd

Move PISTOLS_Cone_Post
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		PitchToTarget

	Type		male
	  Anim          male\dual_guns_cone 86
	  PitchAngle	0.2
	  PitchEnd	55
	TEnd
MEnd


###############################
####### PISTOLS 3 shot  ######
###############################

Move PISTOLS_3Shot_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SPAWNHIP BURST
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_3Shot
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move GunslingerDual_3shot_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS BURST SPAWNHIP GUNSLINGER
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_3Shot
	Sets            CANTMOVE

	Type		male
	  Anim          male\dual_gunslinger_draw 1 30
	TEnd
MEnd

#############################################################

Move PISTOLS_3Shot
	Requires	COMBAT WEAPON DUAL PISTOLS BURST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PISTOLS_3Shot_Post
	Sets            CANTMOVE

	Type		male
	  Anim          male\dual_guns_3_shot 1 51
	TEnd
MEnd

Move PISTOLS_3Shot_Post
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready

	Type		male
	  Anim          male\dual_guns_3_shot 52
	TEnd
MEnd

#############################################################

Move A_PISTOLS_3Shot_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SPAWNHIP BURST
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_PISTOLS_3Shot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_PISTOLS_3Shot
	Requires	FLY COMBAT WEAPON DUAL PISTOLS BURST
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_PISTOLS_3Shot_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE PitchToTarget

	Type		male
	  Anim          male\air_dual_guns_3_shot 1 51
	TEnd
MEnd

Move A_PISTOLS_3Shot_Post
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale PitchToTarget

	Type		male
	  Anim          male\air_dual_guns_3_shot 52
	  PitchEnd	58
	TEnd
MEnd


#############################################################
## Dual Gunslinger Guns
#############################################################

## DUAL GUNSLINGER DRAW ###########################################################

Move Draw_GunslingerDual
	Requires	COMBAT WEAPON DUAL PISTOLS SPAWNHIP GUNSLINGER
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        GunslingerDual_Cycle
	Sets            CANTMOVE

	Type		male
	  Anim          male\dual_gunslinger_draw 1 30
	TEnd
MEnd

Move GunslingerDual_Cycle
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\dual_cycle 1 31
	TEnd
MEnd


## DUALMGUNS DUAL SHOOT CLIP ###########################################################

Move DualMguns_ShootClip_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SPAWNHIP SHOOTCLIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_ShootClip
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move DualMguns_ShootClip
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOTCLIP
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_ShootClip_Post
	Flags		CANTMOVE NOSIZESCALE

	Type		male
	  Anim          male\Thug_ShootClip 1 100
	TEnd
MEnd

Move DualMguns_ShootClip_Post
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\Thug_ShootClip 101
	TEnd
MEnd

#############################################################

Move A_DualMguns_ShootClip_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SPAWNHIP SHOOTCLIP
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_ShootClip
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_DualMguns_ShootClip
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOTCLIP
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_ShootClip_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\Thug_Air_ShootClip 1 100
	TEnd
MEnd

Move A_DualMguns_ShootClip_Post
	Requires	FLY COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Thug_Air_ShootClip 101
	TEnd
MEnd


###############################
####### DualMguns DualCone  ######
###############################

Move DualMguns_DualCone_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS CONE TWO SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_DualCone
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move DualMguns_DualCone
	Requires	COMBAT WEAPON DUAL PISTOLS CONE TWO
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_DualCone_Post
	Flags		CANTMOVE PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\dual_mguns_dualcone 1 90
	  PitchAngle	0.2
	  PitchEnd	80
	TEnd
MEnd

Move DualMguns_DualCone_Post
	Requires	COMBAT WEAPON DUAL PISTOLS
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\dual_mguns_dualcone 91
	TEnd
MEnd

#############################################################

Move A_DualMguns_DualCone_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS CONE TWO SPAWNHIP
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_DualCone
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_DualMguns_DualCone
	Requires	FLY COMBAT WEAPON DUAL PISTOLS CONE TWO
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_DualCone_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_dual_mguns_dualcone 1 90
	  PitchAngle	0.2
	  PitchEnd	80
	TEnd
MEnd

Move A_DualMguns_DualCone_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_dual_mguns_dualcone 91
	TEnd
MEnd


###############################
####### DualMguns Cone  ######
###############################

Move DualMguns_Cone_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS CONE SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_Cone
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move DualMguns_Cone
	Requires	COMBAT WEAPON DUAL PISTOLS CONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_Cone_post
	Flags		CANTMOVE PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\dual_mguns_cone 1 90
	  PitchAngle	0.2
	  PitchEnd	85
	TEnd
MEnd

Move DualMguns_Cone_post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\dual_mguns_cone 91
	TEnd
MEnd

#############################################################

Move A_DualMguns_Cone_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS CONE SPAWNHIP
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_Cone
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_DualMguns_Cone
	Requires	FLY COMBAT WEAPON DUAL PISTOLS CONE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_Cone_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_dual_mguns_cone 1 90
	  PitchAngle	0.2
	  PitchEnd	85
	TEnd
MEnd

Move A_DualMguns_Cone_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_dual_mguns_cone 91
	TEnd
MEnd


###############################
####### DualMguns Burst  ######
###############################

Move DualMguns_Burst_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS MACHINEGUN BURST SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_Burst
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move DualMguns_Burst
	Requires	COMBAT WEAPON DUAL PISTOLS MACHINEGUN BURST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_Burst_Post
	Flags		CANTMOVE PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\dual_mguns_burst 1 45
	  PitchAngle	0.2
	TEnd
MEnd

Move DualMguns_Burst_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\dual_mguns_burst 46
	TEnd
MEnd

#############################################################

Move A_DualMguns_Burst_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS MACHINEGUN BURST SPAWNHIP
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_Burst
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_DualMguns_Burst
	Requires	FLY COMBAT WEAPON DUAL PISTOLS MACHINEGUN BURST
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_Burst_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_dual_mguns_burst 1 45
	  PitchAngle	0.2
	TEnd
MEnd

Move A_DualMguns_Burst_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_dual_mguns_burst 46
	TEnd
MEnd


###############################
####### DualMguns HeavyBurst  ######
###############################

Move DualMguns_HeavyBurst_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS HEAVYBURST SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_HeavyBurst
	Sets            CANTMOVE

	Type		male
	  Anim          male\draw_dual_guns 1 30
	TEnd
MEnd

Move DualMguns_HeavyBurst
	Requires	COMBAT WEAPON DUAL PISTOLS HEAVYBURST
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualMguns_HeavyBurst_Post
	Flags		CANTMOVE PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\dual_mguns_heavyburst 1 62
	  PitchAngle	0.2
	TEnd
MEnd

Move DualMguns_HeavyBurst_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\dual_mguns_heavyburst 63
	TEnd
MEnd

#############################################################

Move A_DualMguns_HeavyBurst_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS HEAVYBURST SPAWNHIP
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_HeavyBurst
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\air_draw_dual_guns 1 30
	TEnd
MEnd

Move A_DualMguns_HeavyBurst
	Requires	FLY COMBAT WEAPON DUAL PISTOLS HEAVYBURST
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualMguns_HeavyBurst_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\air_dual_mguns_heavyburst 1 52
	  PitchAngle	0.2
	TEnd
MEnd

Move A_DualMguns_HeavyBurst_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\air_dual_mguns_heavyburst 53
	TEnd
MEnd

##############################################################
### DUAL PISTOLS BLAST - PLAYER POWERS
##############################################################

## DRAW DUAL PISTOLS ###########################################################

Move Draw_DualPistols
	Requires	COMBAT WEAPON DUAL PISTOLS SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        PISTOLS_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\DualPistols_Draw 1
	TEnd
MEnd

#############################################################

Move A_Draw_DualPistols
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SPAWNBACK
	Priority        66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	A_PISTOLS_Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

#############################################################
## DUAL PISTOLS - PISTOLS
#############################################################

Move DualPistols_PistolsA_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_PistolsA
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_PistolsA
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_PistolsA_Post
	Flags		NOSIZESCALE PitchToTarget
	Sets            CANTMOVE
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\DualPistols_Pistol_A 1 30
	  PitchAngle	1.0
	  PitchEnd	25
	TEnd
MEnd

Move DualPistols_PistolsA_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\DualPistols_Pistol_A 31
	TEnd
MEnd

## "PISTOLS" - SINGLE SHOT Air  ###########################################################

Move A_DualPistols_PistolsA_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT SPAWNBACK
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove        A_DualPistols_PistolsA
	Flags		NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_PistolsA
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_PistolsA_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\DualPistols_Pistol_A_air 1 30
	  PitchAngle	1.0
	  PitchEnd	25
	TEnd
MEnd

Move A_DualPistols_PistolsA_Post
	Requires	FLY #COMBAT
	Include 	sequencers\Members\Combat_DualWeapon_air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\DualPistols_Pistol_A_air 30
	TEnd
MEnd

## DualPistols_PistolsB ###########################################################

Move DualPistols_PistolsB
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_PistolsB_Post
	Flags		NOSIZESCALE PitchToTarget
	Sets		CANTMOVE
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\DualPistols_Pistol_B 2 30
	  PitchAngle	-0.2
	  PitchEnd	25
	TEnd
MEnd

Move DualPistols_PistolsB_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\DualPistols_Pistol_B 31
	TEnd
MEnd

## DualPistols_PistolsB Air ###########################################################

Move A_DualPistols_PistolsB
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_PistolsB_Post
	Flags		NOSIZESCALE PitchToTarget
	Sets		CANTMOVE
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\DualPistols_Pistol_B_Air 2 30
	  PitchAngle	-0.2
	  PitchEnd	25
	TEnd
MEnd

Move A_DualPistols_PistolsB_Post
	Requires	FLY #COMBAT
	Include 	sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\DualPistols_Pistol_B_Air 30
	TEnd
MEnd

## DualPistols_PistolsC ###########################################################

Move DualPistols_PistolsC
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_PistolsC_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\DualPistols_Pistol_C 1 30
	  PitchAngle	1.0
	  PitchEnd	25
	TEnd
MEnd

Move DualPistols_PistolsC_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\DualPistols_Pistol_C 31
	TEnd
MEnd

## DualPistols_PistolsC Air ###########################################################

Move A_DualPistols_PistolsC
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_PistolsC_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\DualPistols_Pistol_C_Air 1 30
	  PitchAngle	1.0
	  PitchEnd	25
	TEnd
MEnd

Move A_DualPistols_PistolsC_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\DualPistols_Pistol_C_Air 30
	TEnd
MEnd

## DualPistols_PistolsD ###########################################################

Move DualPistols_PistolsD
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_PistolsD_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\DualPistols_Pistol_D 1 30
	  PitchAngle	-0.2
	  PitchEnd	25
	TEnd
MEnd

Move DualPistols_PistolsD_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs
	Scale		0.5

	Type		male
	  Anim          male\DualPistols_Pistol_D 31
	  PitchAngle	-0.2
	  PitchEnd	25
	TEnd
MEnd

## DualPistols_PistolsD Air ###########################################################

Move A_DualPistols_PistolsD
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_PistolsD_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\DualPistols_Pistol_D_Air 1 30
	TEnd
MEnd

Move A_DualPistols_PistolsD_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\DualPistols_Pistol_D_Air 30
	TEnd
MEnd

#############################################################
## DUAL PISTOLS - DUAL WIELD
#############################################################

Move DualPistols_DualWield_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT STRONG SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualPistols_DualWield
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_DualWield
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT STRONG
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_DualWield_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_DualWield 1 50
	  PitchAngle	-0.1
	  PitchEnd	45
	TEnd
MEnd

Move DualPistols_DualWield_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs
	Scale		0.8

	Type		male
	  Anim          male\DualPistols_DualWield 51
	TEnd
MEnd

#############################################################

Move A_DualPistols_DualWield_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT STRONG SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_DualWield
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_DualWield
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT STRONG
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_DualWield_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_DualWield_Air 1 50
	  PitchAngle	-0.1
	  PitchEnd	45
	TEnd
MEnd

Move A_DualPistols_DualWield_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale
	Scale		0.8

	Type		male
	  Anim          male\DualPistols_DualWield_Air 50
	TEnd
MEnd

#############################################################
## DUAL PISTOLS - EMPTY CLIPS
#############################################################

Move DualPistols_EmptyClips_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT CONE SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_EmptyClips
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_EmptyClips
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT CONE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_EmptyClips_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_EmptyClips 1 50
	  PitchAngle	0.2
	  PitchEnd	50
	TEnd
MEnd

Move DualPistols_EmptyClips_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\DualPistols_EmptyClips 51
	TEnd
MEnd

#############################################################

Move A_DualPistols_EmptyClips_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT CONE SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_EmptyClips
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_EmptyClips
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT CONE
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_EmptyClips_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_EmptyClips_Air 1 50
	  PitchAngle	0.2
	  PitchEnd	50
	TEnd
MEnd

Move A_DualPistols_EmptyClips_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready_Blend
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale
	Scale		0.65

	Type		male
	  Anim          male\DualPistols_EmptyClips_Air 51
	TEnd
MEnd

#############################################################
## AMMO SWAP
#############################################################

#Move DualPistols_AmmoSwap_Draw
#	Requires	COMBAT WEAPON DUAL PISTOLS RELOAD SPAWNBACK
#	Priority        6
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        Pistols_Ready_NoInterpolate #DualPistols_AmmoSwap
#	Flags		NOSIZESCALE
#	Sets            CANTMOVE
#
#	Type		male
#	  Anim          male\DualPistols_Draw 1
#	TEnd
#MEnd

Move DualPistols_AmmoSwap
	Requires	COMBAT WEAPON DUAL PISTOLS RELOAD
	Priority        5
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE
	#Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_AmmoSwap 1 19
	  PlayFX	WEAPONS\DualPistols\SwapAmmo_Attack.fx 1
	TEnd
MEnd

#############################################################

#Move A_DualPistols_AmmoSwap_Draw
#	Requires	FLY COMBAT WEAPON DUAL PISTOLS RELOAD SPAWNBACK
#	Priority        17
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_PISTOLS_Ready #A_DualPistols_AmmoSwap
#	Flags		NOSIZESCALE
#	Sets            CANTMOVE
#
#	Type		male
#	  Anim          male\DualPistols_Draw_Air 1
#	TEnd
#MEnd

Move A_DualPistols_AmmoSwap
	Requires	FLY COMBAT WEAPON DUAL PISTOLS RELOAD
	Priority        16
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	A_PISTOLS_Ready
	Flags		NoSizeScale
	#Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_AmmoSwap_Air 1 19
	  PlayFX	WEAPONS\DualPistols\SwapAmmo_Attack.fx 1
	TEnd
MEnd

#############################################################
## SUPPRESSIVE FIRE
#############################################################

Move DualPistols_SuppressiveFire_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT MID SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_SuppressiveFire
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_SuppressiveFire
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT MID
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_SuppressiveFire_Post
	Flags		PitchToTarget NOSIZESCALE PitchToTarget
	Sets            CANTMOVE
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\DualPistols_SuppressiveFire 1 50
	  PitchAngle	-0.875
	  PitchEnd	40
	TEnd
MEnd

Move DualPistols_SuppressiveFire_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\DualPistols_SuppressiveFire 51
	TEnd
MEnd

#############################################################

Move A_DualPistols_SuppressiveFire_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT MID SPAWNBACK
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_SuppressiveFire
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_SuppressiveFire
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT MID
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_SuppressiveFire_Post
	Flags		PitchToTarget NOSIZESCALE PitchToTarget
	Sets            CANTMOVE
	SticksOnChild   SHOOT RIGHT

	Type		male
	  Anim          male\DualPistols_SuppressiveFire_Air 1 50
	  PitchAngle	-0.875
	  PitchEnd	40
	TEnd
MEnd

Move A_DualPistols_SuppressiveFire_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\DualPistols_SuppressiveFire_Air 51
	  PitchAngle	0.125
	TEnd
MEnd

## DualPistols_SuppressiveFire B ###########################################################

Move DualPistols_SuppressiveFire_B
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT MID
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_SuppressiveFire_B_Post
	Flags		PitchToTarget NOSIZESCALE PitchToTarget
	Sets            CANTMOVE
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\DualPistols_SuppressiveFire_B 1 50
	  PitchAngle	0.875
	  PitchEnd	40
	TEnd
MEnd

Move DualPistols_SuppressiveFire_B_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\DualPistols_SuppressiveFire_B 51
	TEnd
MEnd

#############################################################

Move A_DualPistols_SuppressiveFire_B
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT MID
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_SuppressiveFire_B_Post
	Flags		PitchToTarget NOSIZESCALE PitchToTarget
	Sets            CANTMOVE
	SticksOnChild   SHOOT LEFT

	Type		male
	  Anim          male\DualPistols_SuppressiveFire_B_Air 1 50
	  PitchAngle	0.875
	  PitchEnd	40
	TEnd
MEnd

Move A_DualPistols_SuppressiveFire_B_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\DualPistols_SuppressiveFire_B_Air 51
	TEnd
MEnd

#############################################################
## DUAL PISTOLS - BULLET RAIN
#############################################################

Move DualPistols_BulletRain_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT LAUNCH SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_BulletRain
	Flags		NOSIZESCALE
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_BulletRain
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT LAUNCH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_BulletRain_Post
	Flags		NOSIZESCALE
	Sets            CANTMOVE
	Interpolate	0

	Type		male
	  Anim          male\DualPistols_BulletRain 1 50
	TEnd

#	Type		fem
#	  Anim          fem\DualPistols_BulletRain 1 72
#	TEnd
MEnd

Move DualPistols_BulletRain_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs
	Interpolate	0

	Type		male
	  Anim          male\DualPistols_BulletRain 51
	TEnd

#	Type		fem
#	  Anim          fem\DualPistols_BulletRain 73
#	TEnd
MEnd

#############################################################

Move A_DualPistols_BulletRain_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT LAUNCH SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_BulletRain
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_BulletRain
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT LAUNCH
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_BulletRain_Post
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_BulletRain_Air 1 50
	TEnd
MEnd

Move A_DualPistols_BulletRain_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale
	Scale		0.65

	Type		male
	  Anim          male\DualPistols_BulletRain_Air 51
	TEnd
MEnd

#############################################################
## DUAL PISTOLS - EXECUTIONER'S SHOT
#############################################################

Move DualPistols_Executioner_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS EXECUTE SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_Executioner
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_Executioner
	Requires	COMBAT WEAPON DUAL PISTOLS EXECUTE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_Executioner_Post
	Flags		NOSIZESCALE PitchToTarget
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Executioner 1 50
	  PitchStart	20
	  PitchAngle	-0.75
	  PitchEnd	70
	TEnd
MEnd

Move DualPistols_Executioner_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\DualPistols_Executioner 51
	TEnd
MEnd

#############################################################

Move A_DualPistols_Executioner_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS EXECUTE SPAWNBACK
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_Executioner
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_Executioner
	Requires	FLY COMBAT WEAPON DUAL PISTOLS EXECUTE
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_Executioner_Post
	Flags		NOSIZESCALE PitchToTarget
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Executioner_Air 1 50
	  PitchStart	20
	  PitchAngle	-0.75
	  PitchEnd	70
	TEnd
MEnd

Move A_DualPistols_Executioner_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale
	Scale		0.75

	Type		male
	  Anim          male\DualPistols_Executioner_Air 51
	TEnd
MEnd

#############################################################
## DUAL PISTOLS - PIERCING ROUNDS
#############################################################

Move DualPistols_PiercingRounds_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT SNIPERBLAST SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_PiercingRounds
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_PiercingRounds
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT SNIPERBLAST
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_PiercingRounds_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_PiercingRounds 1 75
	  PitchStart	35
	  PitchAngle	-0.25
	  PitchEnd	70
	TEnd
MEnd

Move DualPistols_PiercingRounds_Post
	#Requires	COMBAT
	Include 	sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready_NoInterpolate
	Flags		NOSIZESCALE ReqInputs
	Scale		0.625

	Type		male
	  Anim          male\DualPistols_PiercingRounds 76
	TEnd
MEnd

#############################################################

Move A_DualPistols_PiercingRounds_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT SNIPERBLAST SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_PiercingRounds
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_PiercingRounds
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT SNIPERBLAST
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_PiercingRounds_Post
	Flags		PitchToTarget NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_PiercingRounds_Air 1 75
	  PitchStart	35
	  PitchAngle	-0.25
	  PitchEnd	70
	TEnd
MEnd

Move A_DualPistols_PiercingRounds_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale
	Scale		0.625

	Type		male
	  Anim          male\DualPistols_PiercingRounds_Air 76
	TEnd
MEnd

#############################################################
## DUAL PISTOLS - HAIL OF BULLETS
#############################################################

Move DualPistols_HailBullets_Draw
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT SPIN SPAWNBACK
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_HailBullets_Pre
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw 1 26
	TEnd
MEnd

Move DualPistols_HailBullets_Pre
	Requires	COMBAT WEAPON DUAL PISTOLS SHOOT SPIN
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        DualPistols_HailBullets
	Flags		NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild	SHOOT SPIN

	Type		male
	  Anim          male\DualPistols_HailBullets 1 105
	TEnd
MEnd

Move DualPistols_HailBullets
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualPistols_HailBullets_Post
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_HailBullets 106 125
	TEnd
MEnd

Move DualPistols_HailBullets_Post
	#Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        Pistols_Ready #_NoInterpolate
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\DualPistols_PiercingRounds 126
	TEnd
MEnd

#############################################################

Move A_DualPistols_HailBullets_Draw
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT SPIN SPAWNBACK
	Priority        68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_HailBullets_Pre
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_Draw_Air 1 26
	TEnd
MEnd

Move A_DualPistols_HailBullets_Pre
	Requires	FLY COMBAT WEAPON DUAL PISTOLS SHOOT SPIN
	Priority        17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> #<AttackIRQ>
	NextMove        A_DualPistols_HailBullets
	Flags		NOSIZESCALE
	Sets            CANTMOVE
	SticksOnChild	SHOOT SPIN

	Type		male
	  Anim          male\DualPistols_HailBullets_Air 1 105
	TEnd
MEnd

Move A_DualPistols_HailBullets
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_DualPistols_HailBullets_Post
	Flags		NOSIZESCALE
	Sets            CANTMOVE

	Type		male
	  Anim          male\DualPistols_HailBullets_Air 106 125
	TEnd
MEnd

Move A_DualPistols_HailBullets_Post
	Requires	FLY #COMBAT
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	CycleMove	A_PISTOLS_Ready
	NextMove	DualWield_PreFall
	Flags		Cycle NoSizeScale
	Scale		0.65

	Type		male
	  Anim          male\DualPistols_HailBullets_Air 126
	TEnd
MEnd