#############################################################
## Weapons - Dual mode - Katana
#############################################################

#Move Ready_To_Katana
#	Requires	COMBAT WEAPON DUAL REV KATANA
#	Priority        5
#	Interrupts	<Ready> <Combat> <WepCombat>
#	Include sequencers\Members\Combat_Weapon_Lefthand.inc
#	NextMove        Katana_Half
#	Flags		FixArmReg
#	SticksOnChild	WEPSPAWN
#
#	Type		male
#	  Anim          male\katana_draw 1 57
#	TEnd
#
#	Type		fem
#	  Anim          fem\katana_draw 1 57
#	TEnd
#
#	Type		huge
#	  Anim          huge\katana_draw 1 57
#	TEnd
#MEnd

#############################################################

Move Katana_Ready
	Requires	COMBAT WEAPON DUAL REV
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	Flags		Cycle FixArmReg
	NextMove        postReady_back
	Scale           0.50

	Type		male
	  Anim          male\katana_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\katana_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\katana_cycle 1 31
	TEnd
MEnd

#############################################################

Move Katana_Half
	Requires	COMBAT WEAPON DUAL REV
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_Ready
	Scale           0.50
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\katana_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\katana_cycle 15 31
	TEnd
MEnd


## DRAW KATANA ###########################################################

Move Draw_Katana
	Requires	COMBAT WEAPON DUAL REV KATANA SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Draw_katana_post
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 20
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 20
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 20
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Draw_katana_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_draw 21 57
	TEnd

	Type		fem
	  Anim          fem\katana_draw 21 57
	TEnd

	Type		huge
	  Anim          huge\katana_draw 21 54
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 21
	TEnd
MEnd


## DRAW NINJA-TO ###########################################################

Move Draw_NinjaTo
	Requires	COMBAT WEAPON DUAL REV KATANA NINJASWORD SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Draw_NinjaTo_post
	Interpolate	8
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd

	#Type		huge
	#  Anim          huge\ninjasword_draw 1 20
	#TEnd
MEnd

Move Draw_NinjaTo_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\ninjasword_draw 21
	TEnd

	#Type		huge
	#  Anim          huge\ninjasword_draw 21
	#TEnd
MEnd


## SLASH ###########################################################

Move NinjaTo_Slash_Draw
	Requires	NINJASWORD COMBAT WEAPON DUAL REV KATANA SLASH SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Slash
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Slash_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA SLASH SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Slash
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Slash
	Requires	COMBAT WEAPON DUAL REV KATANA SLASH
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Slash_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_2slash 1 20
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 7
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 15
	TEnd

	Type		fem
	  Anim          fem\katana_2slash 1 20
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 7
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 15
	TEnd

	Type		huge
	  Anim          huge\katana_2slash 1 20
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 7
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 15
	TEnd
MEnd

Move Katana_Slash_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_2slash 21
	TEnd

	Type		fem
	  Anim          fem\katana_2slash 21
	TEnd

	Type		huge
	  Anim          huge\katana_2slash 21
	TEnd
MEnd


## HACK ###########################################################

Move NinjaTo_Hack_Draw
	Requires	NINJASWORD COMBAT WEAPON DUAL REV KATANA CHOP SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Hack
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Hack_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA CHOP SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Hack
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Hack
	Requires	COMBAT WEAPON DUAL REV KATANA CHOP
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		FixArmReg
	NextMove        Katana_Hack_Post

	Type		male
	  Anim          male\katana_hack 1 36
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		fem
	  Anim          fem\katana_hack 1 36
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		huge
	  Anim          huge\katana_hack 1 36
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd
MEnd

Move Katana_Hack_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_hack 37 58
	TEnd

	Type		fem
	  Anim          fem\katana_hack 37 55
	TEnd

	Type		huge
	  Anim          huge\katana_hack 37 55
	TEnd
MEnd


## SLICE ###########################################################

Move NinjaTo_Slice_Draw
	Requires	NINJASWORD COMBAT WEAPON DUAL REV KATANA CONE SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Slice
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Slice_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA CONE SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Slice
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Slice
	Requires	COMBAT WEAPON DUAL REV KATANA CONE
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Slice_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_slice 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		fem
	  Anim          fem\katana_slice 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		huge
	  Anim          huge\katana_slice 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd
MEnd

Move Katana_Slice_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_slice 36 56
	TEnd

	Type           fem
	  Anim         fem\katana_slice 36 55
	TEnd

	Type           huge
	  Anim         huge\katana_slice 36 55
	TEnd
MEnd


## PARRY ###########################################################

Move NinjaTo_Parry_Draw
	Requires	NINJASWORD COMBAT WEAPON DUAL REV KATANA PARRY SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Parry
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Parry_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA PARRY SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Parry
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Parry
	Requires	COMBAT WEAPON DUAL REV KATANA PARRY
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Parry_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_parry 1 40
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		fem
	  Anim          fem\katana_parry 1 40
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		huge
	  Anim          huge\katana_parry 1 40
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd
MEnd

Move Katana_Parry_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_parry 41 61
	TEnd

	Type		fem
	  Anim          fem\katana_parry 41 60
	TEnd

	Type		huge
	  Anim          huge\katana_parry 41 60
	TEnd
MEnd


## SPINJUMP ###########################################################

Move NinjaTo_SpinJump_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA SPIN NINJASWORD SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_SpinJump
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_SpinJump_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA SPIN SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_SpinJump
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_SpinJump
	Requires	COMBAT WEAPON DUAL REV KATANA SPIN
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_SpinJump_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_spinjump 10 64
	TEnd

	Type		fem
	  Anim          fem\katana_spinjump 10 64
	TEnd

	Type		huge
	  Anim          huge\katana_spinjump 10 64
	TEnd
MEnd

Move Katana_SpinJump_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_spinjump 65
	TEnd

	Type		fem
	  Anim          fem\katana_spinjump 65
	TEnd

	Type		huge
	  Anim          huge\katana_spinjump 65
	TEnd
MEnd


## REVERSE SWEEP ###########################################################

Move NinjaTo_RevSweep_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA SWEEPATTACK NINJASWORD SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_RevSweep
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_RevSweep_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA SWEEPATTACK SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_RevSweep
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_RevSweep
	Requires	COMBAT WEAPON DUAL REV KATANA SWEEPATTACK
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_RevSweep_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_reverse_sweep 1 40
	  PlayFX	powers\MedievalWeapons\Swordtwirl4.fx
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing2.fx 18
	TEnd

	Type		fem
	  Anim          fem\katana_reverse_sweep 1 40
	  PlayFX	powers\MedievalWeapons\Swordtwirl4.fx
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing2.fx 18
	TEnd

	Type		huge
	  Anim          huge\katana_reverse_sweep 1 40
	  PlayFX	powers\MedievalWeapons\Swordtwirl4.fx
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing2.fx 18
	TEnd
MEnd

Move Katana_RevSweep_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_reverse_sweep 41 59
	TEnd

	Type		fem
	  Anim          fem\katana_reverse_sweep 41 57
	TEnd

	Type		huge
	  Anim          huge\katana_reverse_sweep 41 57
	TEnd
MEnd


## HEADSPLITTER ###########################################################

Move NinjaTo_Headsplit_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA CHOP FIERCE NINJASWORD SPAWNHIP
	Priority        59
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Headsplit
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Headsplit_Draw
	Requires	COMBAT WEAPON DUAL REV SPAWNHIP KATANA CHOP FIERCE
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Headsplit
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move Katana_Headsplit
	Requires	COMBAT WEAPON DUAL REV KATANA CHOP FIERCE
	Priority        7
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Headsplit_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_headsplit 1 55
	  PlayFX	powers\MedievalWeapons\Swordtwirl5.fx
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		fem
	  Anim          fem\katana_headsplit 1 55
	  PlayFX	powers\MedievalWeapons\Swordtwirl5.fx
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		huge
	  Anim          huge\katana_headsplit 1 55
	  PlayFX	powers\MedievalWeapons\Swordtwirl5.fx
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd
MEnd

Move Katana_Headsplit_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_headsplit 56 74
	TEnd

	Type		fem
	  Anim          fem\katana_headsplit 56 73
	TEnd

	Type		huge
	  Anim          huge\katana_headsplit 56 72
	TEnd
MEnd


## PUMMEL ###########################################################

Move Katana_brawl
	Requires	COMBAT WEAPON DUAL REV PUNCH BRAWL
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_brawl_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_pummel 1 15
	TEnd

	Type		fem
	  Anim          fem\katana_pummel 1 15
	TEnd

	Type		huge
	  Anim          huge\katana_pummel 1 15
	TEnd
MEnd

Move Katana_brawl_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_pummel 16 49
	TEnd

	Type		fem
	  Anim          fem\katana_pummel 16 47
	TEnd

	Type		huge
	  Anim          huge\katana_pummel 16 47
	TEnd
MEnd


## BOOSTUP ###########################################################

Move Katana_Boostup
	Requires	COMBAT WEAPON DUAL REV BOOSTUP
	Priority        5
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        Katana_Boostup_post

	Type		male
	  Anim          male\absorb 1 60
	TEnd

	Type		huge
	  Anim          huge\boostup 1 60
	TEnd

	Type		fem
	  Anim          fem\boostup 1 60
	TEnd
MEnd

Move Katana_Boostup_post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove	Katana_half
	Interpolate	8
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\katana_taunt 41 66
	TEnd

	Type		huge
	  Anim          huge\katana_taunt 41 66
	TEnd

	Type		fem
	  Anim          fem\katana_taunt 41 66
	TEnd
MEnd

## BACKSTAB ###########################################################

Move Katana_BackstabWindup_Draw
	Requires	COMBAT WEAPON DUAL REV WINDUPNINJATOBACKSTAB SPAWNHIP
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <MoveIRQ>
	NextMove        NinjaTo_BackstabWindup
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move NinjaTo_BackstabWindup_Draw
	Requires	COMBAT WEAPON DUAL REV NINJASWORD WINDUPNINJATOBACKSTAB SPAWNHIP
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <MoveIRQ>
	NextMove        NinjaTo_BackstabWindup
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

#############################################################

Move NinjaTo_BackstabWindup
	Requires	COMBAT WEAPON DUAL REV NINJASWORD WINDUPNINJATOBACKSTAB
	Priority        6
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups.inc
	NextMove        NinjaTo_BackstabWindup_Cycle
	Flags		FixArmReg

	Type		male
	  Anim          male\NinjaTo_backstab 1 69
	TEnd

	Type		fem
	  Anim          fem\NinjaTo_backstab 1 69
	TEnd

	Type		huge
	  Anim          huge\NinjaTo_backstab 1 69
	TEnd
MEnd

Move NinjaTo_BackstabWindup_Cycle
	Requires	COMBAT WEAPON DUAL REV WINDUPNINJATOBACKSTAB
	Include sequencers\Members\Windups.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\NinjaTo_backstab 75 125
	TEnd

	Type		fem
	  Anim          fem\NinjaTo_backstab 75 125
	TEnd

	Type		huge
	  Anim          huge\NinjaTo_backstab 75 125
	TEnd
MEnd

#############################################################

Move Katana_BackstabWindup_Quick_Draw
	Requires	COMBAT WEAPON DUAL REV NINJATOBACKSTAB SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <MoveIRQ>
	NextMove        NinjaToBackstab
	Flags		FixArmReg

	Type		male
	  Anim          male\katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\katana_draw 1 30
	TEnd

	Type		Tsoo
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move NinjaTo_BackstabWindup_Quick_Draw
	Requires	COMBAT WEAPON DUAL REV NINJASWORD NINJATOBACKSTAB SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <MoveIRQ>
	NextMove        NinjaToBackstab
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 30
	TEnd
MEnd

Move NinjaToBackstab
	Requires	COMBAT WEAPON DUAL REV NINJATOBACKSTAB
	Priority        5
	Interrupts	<AttackIRQ> NinjaTo_BackstabWindup NinjaTo_BackstabWindup_Cycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        NinjaToBackstab2
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\NinjaTo_backstab 130 161
	TEnd

	Type		fem
	  Anim          fem\NinjaTo_backstab 130 161
	TEnd

	Type		huge
	  Anim          huge\NinjaTo_backstab 130 161
	TEnd
MEnd

Move NinjaToBackstab2
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        NinjaToBackstab_Postcycle
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\NinjaTo_backstab 162 230
	TEnd

	Type		fem
	  Anim          fem\NinjaTo_backstab 162 230
	TEnd

	Type		huge
	  Anim          huge\NinjaTo_backstab 162 230
	TEnd
MEnd

Move NinjaToBackstab_Postcycle
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\NinjaTo_backstab 230 280
	TEnd

	Type		fem
	  Anim          fem\NinjaTo_backstab 230 280
	TEnd

	Type		huge
	  Anim          huge\NinjaTo_backstab 230 280
	TEnd
MEnd


## PLACATE ###########################################################

Move Katana_Placate_Draw
	Requires	COMBAT WEAPON DUAL REV KATANA NINJASWORD PLACATE SPAWNHIP
	Priority        58
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Placate
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\ninjasword_draw 1 20
	TEnd
MEnd

Move Katana_Placate
	Requires	COMBAT WEAPON DUAL REV KATANA NINJASWORD PLACATE
	Priority        57
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Katana_Placate_Post
	Sets            CANTMOVE

	Type		male
	  Anim          male\katana_placate 1 40
	TEnd
MEnd

Move Katana_Placate_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		male
	  Anim          male\katana_placate 41
	TEnd
MEnd