#############################################################
## Weapons - Dual mode - Katana (Flying)
#############################################################

## AIR KATANA READY ###########################################################

Move A_Katana_Ready
	Requires	FLY COMBAT WEAPON DUAL REV
	Priority	15
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	Flags		Cycle FixArmReg
	NextMove        D_PreFall
	Scale           0.50

	Type		male
	  Anim          male\air_katana_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\air_katana_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\air_katana_cycle 1 31
	TEnd
MEnd

Move A_Katana_half
	Requires	COMBAT WEAPON DUAL REV FLY
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\air_katana_cycle 15 31
	TEnd

	Type		fem
	  Anim          fem\air_katana_cycle 15 31
	TEnd

	Type		huge
	  Anim          huge\air_katana_cycle 15 31
	TEnd
MEnd

## AIR DRAW NINJA-TO ###########################################################

Move A_Draw_NinjaTo
	Requires	FLY COMBAT WEAPON DUAL REV NINJASWORD SPAWNHIP
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<ReactIRQ> <HitIRQ> <DeathIRQ> <AttackIRQ>
	NextMove        A_Draw_NinjaTo_post
	Interpolate	8
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 20
	TEnd

	#Type		huge
	#  Anim          huge\air_ninjasword_draw 1 20
	#TEnd
MEnd

Move A_Draw_NinjaTo_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_ninjasword_draw 21
	TEnd

	#Type		huge
	#  Anim          huge\air_ninjasword_draw 21
	#TEnd
MEnd

## AIR DRAW KATANA ###########################################################

Move A_Draw_Katana
	Requires	FLY COMBAT WEAPON DUAL REV SPAWNHIP KATANA
	Priority	67
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Draw_katana_post
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 20
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 20
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 20
	TEnd
MEnd

Move A_Draw_katana_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_draw 21 57
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 21 57
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 21 57
	TEnd
MEnd


## AIR BRAWL ###########################################################

Move A_Katana_brawl
	Requires	COMBAT WEAPON DUAL REV PUNCH FLY BRAWL
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_brawl_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_pummel 1 15
	TEnd

	Type		fem
	  Anim          fem\air_katana_pummel 1 15
	TEnd

	Type		huge
	  Anim          huge\air_katana_pummel 1 15
	TEnd
MEnd

Move A_Katana_brawl_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_pummel 16 ##49
	TEnd

	Type		fem
	  Anim          fem\air_katana_pummel 16 ##49
	TEnd

	Type		huge
	  Anim          huge\air_katana_pummel 16 ##49
	TEnd
MEnd


## AIR BOOSTUP ###########################################################

Move A_Katana_Boostup
	Requires	COMBAT WEAPON DUAL REV BOOSTUP FLY
	Priority	16
	Interrupts	<AttackIRQ>  ##<ReactIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Boostup_post
	Flags		FixArmReg

	Type		male
	  Anim          male\air_boostup 1 60
	TEnd

	Type		huge
	  Anim          huge\air_boostup 1 60
	TEnd

	Type		fem
	  Anim          fem\air_boostup 1 60
	TEnd
MEnd

Move A_Katana_Boostup_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove	A_Katana_Ready
	Interpolate	8
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_taunt 41 ##66
	TEnd

	Type		huge
	  Anim          huge\air_katana_taunt 41 ##66
	TEnd

	Type		fem
	  Anim          fem\air_katana_taunt 41 ##66
	TEnd
MEnd


## AIR SLASH ###########################################################

Move A_NinjaTo_Slash_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SLASH SPAWNHIP NINJASWORD
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Slash
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_Katana_Slash_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SLASH SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Slash
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_Slash
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SLASH
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Slash_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_slash 1 20
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 7
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 15
	TEnd

	Type		fem
	  Anim          fem\air_katana_slash 1 20
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 7
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 15
	TEnd

	Type		huge
	  Anim          huge\air_katana_slash 1 20
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 7
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 15
	TEnd
MEnd

Move A_Katana_Slash_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_half
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_slash 21
	TEnd

	Type		fem
	  Anim          fem\air_katana_slash 21
	TEnd

	Type		huge
	  Anim          huge\air_katana_slash 21
	TEnd
MEnd


## AIR HACK ###########################################################

Move A_NinjaTo_Hack_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CHOP SPAWNHIP NINJASWORD
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Hack
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_Katana_Hack_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CHOP SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Hack
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_Hack
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CHOP
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Hack_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_hack 1 36
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		fem
	  Anim          fem\air_katana_hack 1 36
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		huge
	  Anim          huge\air_katana_hack 1 36
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd
MEnd

Move A_Katana_Hack_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_hack 37
	TEnd

	Type		fem
	  Anim          fem\air_katana_hack 37
	TEnd

	Type		huge
	  Anim          huge\air_katana_hack 37
	TEnd
MEnd


## SLICE ###########################################################

Move A_NinjaTo_Slice_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CONE SPAWNHIP NINJASWORD
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Slice
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd

MEnd

Move A_Katana_Slice_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CONE SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Slice
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_Slice
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CONE
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Slice_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_slice 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		fem
	  Anim          fem\air_katana_slice 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		huge
	  Anim          huge\air_katana_slice 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd
MEnd

Move A_Katana_Slice_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_slice 36
	TEnd

	Type           fem
	  Anim         fem\air_katana_slice 36
	TEnd

	Type           huge
	  Anim         huge\air_katana_slice 36
	TEnd
MEnd


## AIR PARRY ###########################################################

Move A_NinjaTo_Parry_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA PARRY SPAWNHIP NINJASWORD
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Parry
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_Katana_Parry_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA PARRY SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Parry
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_Parry
	Requires	FLY COMBAT WEAPON DUAL REV KATANA PARRY
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Parry_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_parry 1 40
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		fem
	  Anim          fem\air_katana_parry 1 40
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		huge
	  Anim          huge\air_katana_parry 1 40
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd
MEnd

Move A_Katana_Parry_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_parry 41
	TEnd

	Type		fem
	  Anim          fem\air_katana_parry 41
	TEnd

	Type		huge
	  Anim          huge\air_katana_parry 41
	TEnd
MEnd


## AIR REVERSE SWEEP ###########################################################

Move A_NinjaTo_RevSweep_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SWEEPATTACK NINJASWORD SPAWNHIP
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_RevSweep
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_Katana_RevSweep_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SWEEPATTACK SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_RevSweep
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_RevSweep
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SWEEPATTACK
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_RevSweep_post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_reverse_sweep 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		fem
	  Anim          fem\air_katana_reverse_sweep 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd

	Type		huge
	  Anim          huge\air_katana_reverse_sweep 1 35
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 18
	TEnd
MEnd

Move A_Katana_RevSweep_post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_reverse_sweep 36 ##59
	TEnd

	Type		fem
	  Anim          fem\air_katana_reverse_sweep 36 ##59
	TEnd

	Type		huge
	  Anim          huge\air_katana_reverse_sweep 36 ##59
	TEnd
MEnd


## AIR SPINJUMP ###########################################################

Move A_NinjaTo_SpinJump_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SPIN NINJASWORD SPAWNHIP
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_SpinJump
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_Katana_SpinJump_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SPIN SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_SpinJump
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_SpinJump
	Requires	FLY COMBAT WEAPON DUAL REV KATANA SPIN
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_SpinJump_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_spinjump 10 64
	TEnd

	Type		fem
	  Anim          fem\air_katana_spinjump 10 64
	TEnd

	Type		huge
	  Anim          huge\air_katana_spinjump 10 64
	TEnd
MEnd

Move A_Katana_SpinJump_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_spinjump 65
	TEnd

	Type		fem
	  Anim          fem\air_katana_spinjump 65
	TEnd

	Type		huge
	  Anim          huge\air_katana_spinjump 65
	TEnd
MEnd


## AIR HEADSPLITTER ###########################################################

Move A_NinjaTo_HeadSplitter_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CHOP FIERCE NINJASWORD SPAWNHIP
	Priority	70
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_HeadSplitter
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_Katana_HeadSplitter_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CHOP FIERCE SPAWNHIP
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_HeadSplitter
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_draw 1 30
	TEnd

	Type		fem
	  Anim          fem\air_katana_draw 1 30
	TEnd

	Type		huge
	  Anim          huge\air_katana_draw 1 30
	TEnd
MEnd

Move A_Katana_HeadSplitter
	Requires	FLY COMBAT WEAPON DUAL REV KATANA CHOP FIERCE
	Priority	18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_HeadSplitter_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_katana_headsplit 1 55
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		fem
	  Anim          fem\air_katana_headsplit 1 55
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd

	Type		huge
	  Anim          huge\air_katana_headsplit 1 55
	  PlayFX	powers\MedievalWeapons\zero_SwordSwing.fx 27
	TEnd
MEnd

Move A_Katana_HeadSplitter_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_Katana_Ready
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_katana_headsplit 56
	TEnd

	Type		fem
	  Anim          fem\air_katana_headsplit 56
	TEnd

	Type		huge
	  Anim          huge\air_katana_headsplit 56
	TEnd
MEnd


################################################
##### Air NinjaTo Backstab Windup (ALT sword draw) ####
################################################

Move A_NinjaTo_Backstab_Draw
	Requires	FLY COMBAT WEAPON DUAL REV NINJASWORD WINDUPNINJATOBACKSTAB SPAWNHIP
	Priority	68
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_NinjaTo_Backstab_Windup
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_NinjaTo_Backstab_Windup
	Requires	FLY COMBAT WEAPON DUAL REV NINJASWORD WINDUPNINJATOBACKSTAB
	Priority	67
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_NinjaTo_Backstab_WindupCycle
	Flags		FixArmReg

	Type		male
	  Anim          male\air_NinjaTo_backstab 1 69
	TEnd

	Type		fem
	  Anim          fem\air_NinjaTo_backstab 1 69
	TEnd

	Type		huge
	  Anim          huge\air_NinjaTo_backstab 1 69
	TEnd
MEnd

Move A_NinjaTo_Backstab_WindupCycle
	Requires	FLY COMBAT WEAPON DUAL REV WINDUPNINJATOBACKSTAB
	Include sequencers\Members\Windups_Air.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\air_NinjaTo_backstab 75 125
	TEnd

	Type		fem
	  Anim          fem\air_NinjaTo_backstab 75 125
	TEnd

	Type		huge
	  Anim          huge\air_NinjaTo_backstab 75 125
	TEnd
MEnd

#############################################################

Move A_NinjaTo_Backstab_Quick_Draw
	Requires	FLY COMBAT WEAPON DUAL REV NINJASWORD NINJATOBACKSTAB SPAWNHIP
	Priority	70
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Windups_Air.inc
	NextMove        A_NinjaTo_Backstab
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_NinjaTo_Backstab
	Requires	FLY COMBAT WEAPON DUAL REV NINJATOBACKSTAB
	Priority	16
	Interrupts	<AttackIRQ> A_NinjaTo_Backstab_Windup A_NinjaTo_Backstab_WindupCycle
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_NinjaTo_Backstab_Post
	Sets            CANTMOVE
	Flags		FixArmReg

	Type		male
	  Anim          male\air_NinjaTo_backstab 130 161
	TEnd

	Type		fem
	  Anim          fem\air_NinjaTo_backstab 130 161
	TEnd

	Type		huge
	  Anim          huge\air_NinjaTo_backstab 130 161
	TEnd
MEnd

Move A_NinjaTo_Backstab_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        A_NinjaTo_Backstab_PostCycle
	Flags		FixArmReg ReqInputs

	Type		male
	  Anim          male\air_NinjaTo_backstab 162 227
	TEnd

	Type		fem
	  Anim          fem\air_NinjaTo_backstab 162 227
	TEnd

	Type		huge
	  Anim          huge\air_NinjaTo_backstab 162 227
	TEnd
MEnd

Move A_NinjaTo_Backstab_PostCycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	Flags		Cycle FixArmReg

	Type		male
	  Anim          male\air_NinjaTo_backstab 230 280
	TEnd

	Type		fem
	  Anim          fem\air_NinjaTo_backstab 230 280
	TEnd

	Type		huge
	  Anim          huge\air_NinjaTo_backstab 230 280
	TEnd
MEnd


## AIR PLACATE ###########################################################

Move A_Katana_Placate_Draw
	Requires	FLY COMBAT WEAPON DUAL REV KATANA NINJASWORD PLACATE SPAWNHIP
	Priority	69
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Placate
	Flags		FixArmReg

	Type		male
	  Anim          male\air_ninjasword_draw 1 30
	TEnd
MEnd

Move A_Katana_Placate
	Requires	FLY COMBAT WEAPON DUAL REV KATANA NINJASWORD PLACATE
	Priority	18
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Katana_Placate_Post
	Sets            CANTMOVE

	Type		male
	  Anim          male\air_katana_placate 1 40
	TEnd
MEnd

Move A_Katana_Placate_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Weapon_Lefthand_Air.inc
	NextMove        D_flycycle
	Flags		ReqInputs

	Type		male
	  Anim          male\air_katana_placate 41
	TEnd
MEnd