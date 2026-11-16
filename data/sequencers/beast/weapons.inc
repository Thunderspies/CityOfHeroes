#############################################################
## weapons.inc
#############################################################

## WEAPON READIES ###########################################################

Move PreWep
	Requires	COMBAT WEAPON
	Priority	2
	Interrupts	<Ready> <Combat>
	Include sequencers\Members\Combat_Weapon.inc
	Flags		ReqInputs
	NextMove        WepReady
	Scale           0.50
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\pre_wep 3
	TEnd
MEnd

#############################################################

Move Wep_half
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		ReqInputs
	NextMove        WepReady
	Scale           0.50
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\wep_cycle 15 31
	TEnd
MEnd

#############################################################

Move WepReady
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		Cycle NoSizeScale
	NextMove        postReady_back
	Scale           0.50

	Type		Beast
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

#############################################################

Move PostWepReady
	Include sequencers\Members\Readies.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\post_wep 1
	TEnd
MEnd

#####################
######## Goblin Draw Sword  ######
#####################
##
##Move Draw_saber
##	Requires	COMBAT WEAPON SPAWNBACK ???
##	Priority	3
##	Interrupts	<Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat>
##	Member		<ReactIRQ> <DeathIRQ> <WepAttack> <AttackIRQ>
##	NextMove        Draw_saber_post
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          male\goblin_draw_saber 1 30
##	TEnd
##MEnd
##
##Move Draw_saber_post
####	Requires	NO BITS   ##COMBAT WEAPON
##	Include sequencers\Members\Combat_Weapon.inc
##	NextMove        Ready
####
##	Type		Beast
##	  Anim          male\goblin_draw_saber 31
##	TEnd
##MEnd
##
###########################
########  Goblin  Sword Slash  ######
###########################
##
##Move 1st_SwordSlash
##	Requires	COMBAT WEAPON AXE SPAWNBACK SLASH ???
##	Priority	6
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_SwordSlash2
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          male\goblin_draw_saber 1 30
##	TEnd
##MEnd
##
##Move 1st_SwordSlash2
##	Requires	COMBAT WEAPON
##	Member		<DeathIRQ>
##	NextMove        1st_SwordSlash_post
##	Priority	2
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          male\goblin_sword_slash 1 50
##	TEnd
##MEnd
##
##Move 1st_SwordSlash_post
##	Requires	COMBAT WEAPON
##	Include sequencers\Members\Combat_Weapon.inc
##	NextMove        Ready
##	Priority	2
##	Interpolate     0
##
##	Type		Beast
##	  Anim          male\goblin_sword_slash 51
##	TEnd
##MEnd
##
#############################################################
##
##Move 2nd_SwordSlash
##	Requires	COMBAT WEAPON AXE SLASH
##	Priority	5
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_SwordSlash_post
##	Sets            CANTMOVE
##	interpolate	8
##
##	Type		Beast
##	  Anim          male\goblin_sword_slash 1 50
##	TEnd
##MEnd
##
##
###########################
######## Goblin Sword Reverse  ######
###########################
##
##Move 1st_SwordRev
##	Requires	COMBAT WEAPON SPAWNBACK SWIPE AXE
##	Priority	6
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_SwordRev2
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          male\goblin_draw_saber 1 30
##	TEnd
##MEnd
##
##Move 1st_SwordRev2
##	Requires	COMBAT WEAPON
##	Priority	2
##	Member		<DeathIRQ>
##	NextMove        1st_SwordRev_post
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          male\goblin_sword_reverse 1 50
##	TEnd
##MEnd
##
##Move 1st_SwordRev_post
##	Requires	COMBAT WEAPON
##	Include sequencers\Members\Combat_Weapon.inc
##	NextMove        Ready
##	Priority	2
##	Interpolate     0
##
##	Type		Beast
##	  Anim          male\goblin_sword_reverse 51
##	TEnd
##
##MEnd
##
#############################################################
##
##Move 2nd_SwordRev
##	Requires	COMBAT WEAPON SWIPE AXE
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_SwordRev_post
##	Priority	5
##	Sets            CANTMOVE
##	interpolate	8
##
##	Type		Beast
##	  Anim          male\goblin_sword_reverse 1 50
##	TEnd
##MEnd

#########################################
### Club smash (can be used for swords) ##
########################################
##
##Move 1st_ClubSmash
##	Requires	COMBAT WEAPON AXE SPAWNBACK CLUBSMASH
##	Priority	5
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_ClubSmash2
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          male\draw_axe 1 30
##	TEnd
##MEnd
##
##
##Move 1st_ClubSmash2
##	Requires	COMBAT WEAPON
##	Member		<ReactIRQ> <DeathIRQ> <WepAttack> <AttackIRQ>
##	NextMove        1st_ClubSmash_post
##	Priority	2
##	Sets            CANTMOVE
##
##
##	Type		Beast
##	  Anim          huge\giant_club_smash 1 40
##	TEnd
##MEnd
##
##Move 1st_ClubSmash_post
##	Requires	COMBAT WEAPON
##	Include sequencers\Members\Combat_Weapon.inc
##	NextMove        Wep_half
##	Priority	2
##	Interpolate     0
##	Flags		NoSizeScale
##
##	Type		Beast
##	  Anim          huge\giant_club_smash 41
##	TEnd
##MEnd
##
#############################################################
##
##Move 2nd_ClubSmash
##	Requires	COMBAT WEAPON AXE CLUBSMASH
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_ClubSmash_post
##	Priority	4
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          huge\giant_club_smash 1 40
##	TEnd
##MEnd

#########################################
### Club Swing (can be used for swords) ##
########################################
##
##
##Move 1st_ClubSwing
##	Requires	COMBAT WEAPON AXE SPAWNBACK CLUBSWING
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_ClubSwing2
##	Priority	5
##	Sets            CANTMOVE
##
##	Type		Beast
##	  Anim          male\draw_axe 1 30
##	TEnd
##
##MEnd
##
##
##Move 1st_ClubSwing2
##	Requires	COMBAT WEAPON
##	Member		<ReactIRQ> <DeathIRQ> <WepAttack> <AttackIRQ>
##	NextMove        1st_ClubSwing_post
##	Priority	2
##	Sets            CANTMOVE
##	Flags		NoSizeScale
##
##
##	Type		Beast
##	  Anim          huge\giant_club_bigswing 1 40
##	TEnd
##
##MEnd
##
##
##Move 1st_ClubSwing_post
##	Requires	COMBAT WEAPON
##	Include sequencers\Members\Combat_Weapon.inc
##	NextMove        Wep_half
##	Priority	2
##	Interpolate     0
##	Flags		NoSizeScale
##
##	Type		Beast
##	  Anim          huge\giant_club_bigswing 41
##	TEnd
##
##MEnd
##
#######
##
##Move 2nd_ClubSwing
##	Requires	COMBAT WEAPON AXE CLUBSWING
##	Interrupts	<AttackIRQ>
##	Member		<DeathIRQ>
##	NextMove        1st_ClubSwing_post
##	Priority	4
##	Sets            CANTMOVE
##	Flags		NoSizeScale
##
##
##	Type		Beast
##	  Anim          huge\giant_club_bigswing 1 40
##	TEnd
##
##MEnd


###################
###### Axe BASIC ATTACK  ######
###################

Move 1st_Axe
	Requires	COMBAT WEAPON CHOP AXE SPAWNBACK
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_Axe2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move 1st_Axe2
	Requires	COMBAT WEAPON CHOP AXE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_Axe_post
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\wm_chop 1 25
	TEnd
MEnd

Move 1st_Axe_post
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\wm_chop 25
	TEnd
MEnd

###################
######Weapon Hit Quick  ######
###################

Move W_HitQuick
	Requires	COMBAT WEAPON QUICK HIT
	Priority	4
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <HitIRQ>
	NextMove        WepReady
	Flags		HitReact
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\wepHQ_high 1
	TEnd
MEnd

###################
### Weapon run Forward ##
###################

Move W_RunPre
	Requires	COMBAT WEAPON FORWARD
	Priority	3
	Interrupts	<WepCombat> <WepAttack> <PostMoveCycle> <Block> <HitIRQ>
	Member		<WepMove> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_RunCycle

	Type		Beast
	  Anim          huge\wep_run 15 25
	  PlayFx        FEET\HUGERIGHT2.FX 6
	TEnd
MEnd

Move W_RunCycle
	Requires	FORWARD COMBAT WEAPON
	Priority	3
	Member		<WepMove> <DeathIRQ> <AttackIRQ>
	Flags		Cycle MoveScale SmoothSprint
	MoveRate        0.7
	NextMove        W_RunPost

	Type		Beast
	  Anim          huge\wep_run 1 25
	  PlayFx        FEET\HUGELEFT2.FX 10
	  PlayFx        FEET\HUGERIGHT2.FX 21
	  SmoothSprint  3.3
	TEnd
MEnd

Move W_RunPost
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	Flags		ReqInputs
	NextMove        WepReady
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\weprun 39
	  PlayFx        FEET\giantland4.FX 3
	  Scale		0.7
	TEnd
MEnd

###################
###### sword BASIC ATTACK  ######
###################

Move Draw_saber
	Requires	COMBAT WEAPON SPAWNBACK
	Priority	3
	Interrupts	<Ready> <PostMoveCycle> <Combat> <WepCombat> <TwoHandCombat>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Draw_saber_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\draw_axe 1 22
	TEnd
MEnd

Move Draw_saber_post
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\draw_axe 22
	TEnd
MEnd

#############################################################

Move 1st_sword
	Requires	COMBAT WEAPON AXE SLASH SPAWNBACK
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_sword2
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move 1st_sword2
	Requires	COMBAT WEAPON AXE SLASH
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_sword_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\sword_1st_slash 1 55
	TEnd
MEnd

Move 1st_sword_post
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\sword_1st_slash 55
	TEnd
MEnd

###################
###### Axe Swoop  ######
###################

Move 1st_AxeSwoop
	Requires	COMBAT WEAPON UPPERCUT AXE SPAWNBACK
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_AxeSwoop2
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move 1st_AxeSwoop2
	Requires	COMBAT WEAPON UPPERCUT AXE
	Priority	4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        1st_AxeSwoop_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\axe_swoop 1 65
	TEnd
MEnd

Move 1st_AxeSwoop_post
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\axe_swoop 65
	TEnd
MEnd

###################
###### Axe Reverse Sweep  ######
###################

Move 1st_Axe_Rsweep
	Requires	COMBAT WEAPON SWIPE AXE SPAWNBACK
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_Axe_Rsweep2
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move 1st_Axe_Rsweep2
	Requires	COMBAT WEAPON SWIPE AXE
	Priority	4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        1st_Axe_Rsweep_post
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\axe_reverse_sweep 1 65
	TEnd
MEnd

Move 1st_Axe_Rsweep_post
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\axe_reverse_sweep 65
	TEnd
MEnd

###################
###### fire sword attack  ######
###################

Move Draw_firesword
	Requires	COMBAT WEAPON WEPSPAWN2
	Priority	3
	Interrupts	<Ready> <Combat> <WepCombat> <TwoHandCombat> <PostMoveCycle>
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Draw_firesword_post

	Type		Beast
	  Anim          male\firesword_draw 1 21
	TEnd
MEnd

Move Draw_firesword_post
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady

	Type		Beast
	  Anim          male\firesword_draw 22
	TEnd
MEnd

#############################################################

Move 1st_Fsword
	Requires	COMBAT WEAPON SWORD ATTACK WEPSPAWN2
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        1st_Fsword2
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\firesword_drawNslash 1 80
	TEnd
MEnd

Move 1st_Fsword2
	Requires	COMBAT WEAPON
	Priority	2
	Member		<DeathIRQ>
	NextMove        Wep_half
	Priority	2
	Flags		NoSizeScale

	Type		Beast
	  Anim		male\firesword_drawNslash 80
	TEnd
MEnd

#############################################################

Move 2nd_Fsword
	Requires	COMBAT WEAPON SWORD ATTACK
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        2nd_Fsword2
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\wm_slash 1 30
	TEnd
MEnd

Move 2nd_Fsword2
	Requires	COMBAT WEAPON
	Priority	2
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Wep_half
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\wm_slash 30
	TEnd
MEnd

#########################################
###### Greater Firesword (old 2Hsword)  ######
#########################################

Move Axeupcut_Draw
	Requires	COMBAT WEAPON SWORD 2HAND ATTACK WEPSPAWN2
	Priority	6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Axeupcut
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\firesword_drawNupcut 1 90
	TEnd
MEnd

Move Axeupcut
	Requires	COMBAT WEAPON SWORD 2HAND ATTACK
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Axeupcut_Post
	Sets            CANTMOVE

	Type		Beast
	  Anim          male\wm_upcut 1 54
	TEnd
MEnd

Move Axeupcut_Post
	Requires	COMBAT
	Priority	1
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        Fake_WepReady_Half
	Flags		NoSizeScale

	Type		Beast
	  Anim          male\wm_upcut 54
	TEnd
MEnd

#############################################################
##  WEAPON FLY READY
#############################################################

Move W_Fall_Fready
	Requires	FLY COMBAT WEAPON
	Priority	3
	Interrupts	W_FallCycle W_PreFall W_Ffly_PreFall W_Ffly_PreFall_2
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        W_flyready

	Type		Beast
	  Anim          male\wep_flydown 180
	TEnd
MEnd

Move W_Preflyready
	Requires	FLY COMBAT WEAPON
	Priority	3
	Interrupts	<WepCombat> <WepCombatMove> <TwoHandCombat> <TwoHandMove> <Jumps> <WepAttack> Preflyready
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        W_flyready

	Type		Beast
	  Anim          male\pre_airwep 2
	TEnd
MEnd

Move air_T_WepCombat
	Requires	FLY COMBAT WEAPON
	Priority	3
	Interrupts	<FlyReady>
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        W_flyready

	Type		Beast
	  Anim          male\air_wepready 2 24
	TEnd
MEnd

#############################################################

Move W_flyready
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		Cycle
	NextMove        W_PreFall
	Wind		FLY

	Type		Beast
	  Anim          male\air_wepready 25 65
	TEnd
MEnd

#############################################################

Move W_flycycle_half
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	NextMove        W_flycycle
	Scale           0.50
	Wind		FLY

	Type		Beast
	  Anim          male\air_wep_cycle 15 31
	TEnd
MEnd

#############################################################

Move W_flycycle
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		Cycle
	NextMove        W_PreFall
	Scale           0.50
	Wind		FLY

	Type		Beast
	  Anim          male\air_wep_cycle 1 31
	TEnd
MEnd

###################
### Wep Hover forward  ####
###################

Move W_Hoverpre
	Requires	FORWARD FLY COMBAT WEAPON HOVER
	Interrupts	<FlyWepCombat> <FlyWepAttack> <WepMove> <PostMoveCycle> <FlyFall> <Jumps>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_HoverCycle
	Priority	5
	Scale           1.50

	Type		Beast
	  Anim          male\wep_hover 1 54
	TEnd
MEnd

Move W_HoverCycle
	Requires	FORWARD FLY COMBAT WEAPON HOVER
	Member		<ReactIRQ> <DeathIRQ> <Flycycle> <AttackIRQ>
	Flags		Cycle
	NextMove        W_HoverPost
	Priority	5

	Type		Beast
	  Anim          male\wep_hover 57 106
	TEnd
MEnd

Move W_HoverPost
	Requires	FLY COMBAT WEAPON HOVER
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		ReqInputs
	NextMove        W_flyready
	Priority	4

	Type		Beast
	  Anim          male\wep_hover 107
	TEnd
MEnd

###################
### Weapon Fly forward  ####
###################

Move W_flypre
	Requires	FORWARD FLY COMBAT WEAPON
	Priority	4
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> W_RunJumpCycle W_RunJumpCycleB W_Ffly_PreFall
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_flyingCycle

	Type		Beast
	  Anim          male\air_wep_fly 4 24
	TEnd
MEnd

Move W_flyingCycle
	Requires	FORWARD FLY COMBAT WEAPON
	Priority	4
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		Cycle
	NextMove        W_flyPost
	Wind		FLY

	Type		Beast
	  Anim          male\air_wep_fly 25 82
	TEnd
MEnd

Move W_flyPost
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		ReqInputs
	NextMove        W_flyready
	Scale           1.30
	Wind		FLY

	Type		Beast
	  Anim          male\air_wep_fly 83
	TEnd
MEnd

###################
## Weapon fly Backward ##
###################

Move W_BackflyPre
	Requires	BACKWARD FLY COMBAT WEAPON
	Priority	4
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove>
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_BackflyCycle

	Type		Beast
	  Anim          male\air_wep_flyback 3 13
	TEnd
MEnd

Move W_BackflyCycle
	Requires	BACKWARD FLY COMBAT WEAPON
	Priority	4
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		Cycle
	NextMove        W_BackflyPost

	Type		Beast
	  Anim          male\air_wep_flyback 14 63
	TEnd
MEnd

Move W_BackflyPost
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		ReqInputs
	NextMove        W_flyready
	Scale           1.30

	Type		Beast
	  Anim          male\air_wep_flyback 64
	TEnd
MEnd

###################
### Weapon Fly Up  ####
###################

Move W_flyUPpre
	Requires	JUMP FLY COMBAT WEAPON
	Priority	4
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> <Jumps>
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_flyUPCycle
	Scale           0.90
	Wind		FLYUP

	Type		Beast
	  Anim          male\air_wep_flyup 5 34
	TEnd
MEnd

Move W_flyUPCycle
	Requires	JUMP FLY COMBAT WEAPON
	Priority	4
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		Cycle
	NextMove        W_flyUPPost
	Wind		FLYUP

	Type		Beast
	  Anim          male\air_wep_flyup 36 93
	TEnd
MEnd

Move W_flyUPPost
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		ReqInputs
	NextMove        W_flyready
	Wind		FLY

	Type		Beast
	  Anim          male\air_wep_flyup 94
	TEnd
MEnd

###################
###### Weapon Fly Down  ######
###################

Move W_PreFall
	Requires	AIR COMBAT WEAPON
	Priority	3
	Member		<ReactIRQ> <DeathIRQ> <Flymove> <AttackIRQ>
	Flags		Cycle
	NextMove        W_JumpPost

	Type		Beast
	  Anim          male\wep_flydown 1
	TEnd
MEnd

###################
###### Weapon Fly Forward Down  ######
###################

Move W_Ffly_PreFall
	Requires	FORWARD AIR COMBAT WEAPON
	Priority	4
	Interrupts	W_flyPost
	Member		<ReactIRQ> <DeathIRQ> <Flymove> <AttackIRQ>
	NextMove        W_Ffly_PreFall_2

	Type		Beast
	  Anim          male\wep_flydown 1 3
	TEnd
MEnd

Move W_Ffly_PreFall_2
	Requires	AIR COMBAT WEAPON
	Priority	3
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ>
	Flags		Cycle
	NextMove        W_JumpPost

	Type		Beast
	  Anim          male\wep_flydown 4
	TEnd
MEnd

###################
### Weapon fly Right ####
###################

Move W_RightflyPre
	Requires	STEPRIGHT FLY COMBAT WEAPON
	Priority	4
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> W_Ffly_Fready
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_RightflyCycle

	Type		Beast
	  Anim          male\air_wep_flyR 3 9
	TEnd
MEnd

Move W_RightflyCycle
	Requires	STEPRIGHT FLY COMBAT WEAPON
	Priority	4
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		Cycle
	NextMove        W_RightflyPost

	Type		Beast
	  Anim          male\air_wep_flyR 10 59
	TEnd
MEnd

Move W_RightflyPost
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		ReqInputs
	NextMove        W_flyready

	Type		Beast
	  Anim          male\air_wep_flyR 60
	TEnd
MEnd

###################
### Weapon fly Left  ####
###################

Move W_LeftflyPre
	Requires	STEPLEFT FLY COMBAT WEAPON
	Priority	4
	Interrupts	<FlyWepCombat> <FlyWepAttack> <PostFLYCycle> <WepMove> W_Ffly_Fready
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_LeftflyCycle

	Type		Beast
	  Anim          male\air_wep_flyL 3 9
	TEnd
MEnd

Move W_LeftflyCycle
	Requires	STEPLEFT FLY COMBAT WEAPON
	Priority	4
	Member		<ReactIRQ> <DeathIRQ> <FlyWepMove> <AttackIRQ>
	Flags		Cycle
	NextMove        W_LeftflyPost

	Type		Beast
	  Anim          male\air_wep_flyL 10 59
	TEnd
MEnd

Move W_LeftflyPost
	Requires	FLY COMBAT WEAPON
	Priority	3
	Include sequencers\Members\Combat_Weapon_Air.inc
	Flags		ReqInputs
	NextMove        W_flyready

	Type		Beast
	  Anim          male\air_wep_flyL 60
	TEnd

MEnd

###################
###### Falling from Weapon run  ######
###################

Move W_RunFall
	Requires	BIGFALL WEAPON
	Priority	2
	Interrupts	<WepMove>
	Member		<ReactIRQ> <DeathIRQ> <FlyFall> <AttackIRQ>
	Flags		ReqInputs
	NextMove        W_RunFall_2

	Type		Beast
	  Anim          male\wep_superjump 67 89
	TEnd
MEnd

Move W_RunFall_2
	Requires	AIR WEAPON
	Priority	2
	Member		<ReactIRQ> <DeathIRQ> <FlyFall> <AttackIRQ>
	Flags		Cycle
	NextMove        W_JumpPost

	Type		Beast
	  Anim          male\wep_superjump 89
	TEnd
MEnd