#############################################################
## Shields_Weapons.inc - Shield\Weapon sequences
#############################################################

## DRAW WEAPON (BACK) ###########################################################

Move Shield_WeaponDraw_Back
	Requires	COMBAT WEAPON SPAWNBACK SHIELD
	Priority	64
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Blend
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_WeaponDrawBack 1
	TEnd
MEnd

#############################################################

Move A_Shield_WeaponDraw_Back
	Requires	FLY COMBAT WEAPON SPAWNBACK SHIELD
	Priority	75
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatReady_Blend
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_Air_WeaponDrawBack 1
	TEnd
MEnd


## CHOP ###########################################################

Move Shield_Chop_Draw
	Requires	COMBAT WEAPON CHOP AXE SHIELD SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Chop
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Chop
	Requires	COMBAT WEAPON CHOP AXE SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Chop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponChop 1 38
	TEnd
MEnd

Move Shield_Chop_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponChop 39
	TEnd
MEnd

#############################################################

Move A_Shield_Chop_Draw
	Requires	FLY COMBAT WEAPON CHOP AXE SHIELD SPAWNBACK
	Priority	79
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Chop
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Chop
	Requires	FLY COMBAT WEAPON CHOP AXE SHIELD
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Chop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponChop 1 40
	TEnd
MEnd

Move A_Shield_Chop_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponChop 41
	TEnd
MEnd


## HAMMER SMASH ###########################################################

Move Shield_HammerSmash_Pre
	Requires	SHIELD COMBAT WEAPON CLUB SMASH
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_HammerSmash
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.9

	Type		male
	  Anim		male\shield_WeaponChop 1 40
	TEnd
MEnd

Move Shield_HammerSmash
	Requires	COMBAT SHIELD
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_HammerSmash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_WeaponChop 41 46
	TEnd
MEnd

Move Shield_HammerSmash_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponChop 47
	TEnd
MEnd

#############################################################

Move A_Shield_HammerSmash_Pre
	Requires	FLY SHIELD COMBAT WEAPON CLUB SMASH
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_HammerSmash
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.9

	Type		male
	  Anim		male\shield_Air_WeaponChop 1 40
	TEnd
MEnd

Move A_Shield_HammerSmash
	#Requires	FLY COMBAT SHIELD
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_HammerSmash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE #ReqInputs
	Scale		1.8

	Type		male
	  Anim		male\shield_Air_WeaponChop 41 46
	TEnd
MEnd

Move A_Shield_HammerSmash_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_DualWeapon_Air.inc
	NextMove	Shield_CombatFly_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponChop 47
	TEnd
MEnd

## SLASH ###########################################################

Move Shield_Slash_Draw
	Requires	COMBAT WEAPON AXE SLASH MID SHIELD SPAWNBACK
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Slash
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Slash
	Requires	COMBAT WEAPON AXE SLASH MID SHIELD
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_slash 1 41
	TEnd
MEnd

Move Shield_Slash_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_slash 42
	TEnd
MEnd

#############################################################

Move A_Shield_Slash_Draw
	Requires	FLY COMBAT WEAPON AXE SLASH MID SHIELD SPAWNBACK
	Priority	78
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Slash
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Slash
	Requires	FLY COMBAT WEAPON AXE SLASH MID SHIELD
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Slash_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponSlash 1 41
	TEnd
MEnd

Move A_Shield_Slash_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Blend
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponSlash 42
	TEnd
MEnd


## REVERSE SWEEP ###########################################################

Move Shield_RSweep_Draw
	Requires	COMBAT WEAPON SWIPE AXE SHIELD SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_RSweep
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_RSweep
	Requires	COMBAT WEAPON SWIPE AXE SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_RSweep_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponReverseSweep 1 61
	TEnd
MEnd

Move Shield_RSweep_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponReverseSweep 62
	TEnd
MEnd

#############################################################

Move A_Shield_RSweep_Draw
	Requires	FLY COMBAT WEAPON SWIPE AXE SHIELD SPAWNBACK
	Priority	77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_RSweep
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_RSweep
	Requires	FLY COMBAT WEAPON SWIPE AXE SHIELD
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_RSweep_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponReverseSweep 1 65
	TEnd
MEnd

Move A_Shield_RSweep_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponReverseSweep 66
	TEnd
MEnd


## PARRY ###########################################################

Move Shield_Parry_Draw
	Requires	COMBAT WEAPON AXE PARRY SHIELD SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Parry
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Parry
	Requires	COMBAT WEAPON AXE PARRY SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Parry_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponParry 1 45
	TEnd
MEnd

Move Shield_Parry_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponParry 46
	TEnd
MEnd

#############################################################

Move A_Shield_Parry_Draw
	Requires	FLY COMBAT WEAPON AXE PARRY SHIELD SPAWNBACK
	Priority	77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Parry
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Parry
	Requires	FLY COMBAT WEAPON AXE PARRY SHIELD
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Parry_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponParry 1 45
	TEnd
MEnd

Move A_Shield_Parry_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponParry 46
	TEnd
MEnd


## SWOOP ###########################################################

Move Shield_Swoop_Draw
	Requires	COMBAT WEAPON UPPERCUT AXE SHIELD SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Swoop
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Swoop
	Requires	COMBAT WEAPON UPPERCUT AXE SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Swoop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponSwoop 1 55
	TEnd
MEnd

Move Shield_Swoop_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponSwoop 56
	TEnd
MEnd

#############################################################

Move A_Shield_Swoop_Draw
	Requires	FLY COMBAT WEAPON UPPERCUT AXE SHIELD SPAWNBACK
	Priority	77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Swoop
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Swoop
	Requires	FLY COMBAT WEAPON UPPERCUT AXE SHIELD
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Swoop_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponSwoop 1 55
	TEnd
MEnd

Move A_Shield_Swoop_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponSwoop 56
	TEnd
MEnd


## CLOBBER ###########################################################

Move Shield_Clobber_Draw
	Requires	COMBAT WEAPON MACE SMASH SHIELD SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Clobber
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Clobber
	Requires	COMBAT WEAPON MACE SMASH SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Clobber_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponClobber 1 71
	TEnd
MEnd

Move Shield_Clobber_Post
	#Requires	COMBAT SHIELD WEAPON MACE
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponClobber 72
	TEnd
MEnd

#############################################################

Move A_Shield_Clobber_Draw
	Requires	FLY COMBAT WEAPON MACE SMASH SHIELD SPAWNBACK
	Priority	77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Clobber
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Clobber
	Requires	FLY COMBAT WEAPON MACE SMASH SHIELD
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	A_Shield_Clobber_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponClobber 1 71
	TEnd
MEnd

Move A_Shield_Clobber_Post
	#Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponClobber 72
	TEnd
MEnd


## UPPERCUT ###########################################################

Move Shield_Uppercut_Draw
	Requires	COMBAT WEAPON UPPERCUT MACE SHIELD SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Uppercut
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Uppercut
	Requires	COMBAT WEAPON UPPERCUT MACE SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Uppercut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponUppercut 1 45
	TEnd
MEnd

Move Shield_Uppercut_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponUppercut 46
	TEnd
MEnd

#############################################################

Move A_Shield_Uppercut_Draw
	Requires	FLY COMBAT WEAPON UPPERCUT MACE SHIELD SPAWNBACK
	Priority	77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Uppercut
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Uppercut
	Requires	FLY COMBAT WEAPON UPPERCUT MACE SHIELD
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Uppercut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponUppercut 1 45
	TEnd
MEnd

Move A_Shield_Uppercut_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponUppercut 46
	TEnd
MEnd


## QUICK UPPERCUT ###########################################################

Move Shield_WeaponUppercut
	Requires	SHIELD COMBAT WEAPON SWORD 2HAND ATTACK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_WeaponUppercut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponUppercut 1 25
	TEnd
MEnd

Move Shield_WeaponUppercut_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponUppercut 26
	TEnd
MEnd

#############################################################

Move A_Shield_WeaponUppercut
	Requires	FLY SHIELD COMBAT WEAPON SWORD 2HAND ATTACK
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_WeaponUppercut_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponUppercut 1 25
	TEnd
MEnd

Move A_Shield_WeaponUppercut_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponUppercut 26
	TEnd
MEnd


## TWIRL ###########################################################

Move Shield_Twirl_Draw
	Requires	COMBAT WEAPON AXE SPIN SHIELD SPAWNBACK
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Twirl
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Twirl
	Requires	COMBAT WEAPON AXE SPIN SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	1st_Twirl_post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponJumpSpin 1 87
	TEnd
MEnd

Move Shield_Twirl_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		ReqInputs

	Type		male
	  Anim		male\shield_WeaponJumpSpin 88
	TEnd
MEnd

#############################################################

Move A_Shield_Twirl_Draw
	Requires	FLY COMBAT WEAPON AXE SPIN SHIELD SPAWNBACK
	Priority	77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Twirl
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Twirl
	Requires	FLY COMBAT WEAPON AXE SPIN SHIELD
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Twirl_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponJumpSpin 1 87
	TEnd
MEnd

Move A_Shield_Twirl_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponJumpSpin 88
	TEnd
MEnd


## JUMPSPIN ###########################################################

Move Shield_Weapon_Jumpspin
	Requires	SHIELD COMBAT WEAPON FIERCE SWORD ATTACK
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Weapon_Jumpspin_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponJumpSpin 1 40
	TEnd
MEnd

Move Shield_Weapon_Jumpspin_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_WeaponJumpSpin 41
	  PlayFx        FEET\LAND1.FX  34
	TEnd

	Type		huge
	  Anim		male\shield_WeaponJumpSpin 41
	  PlayFx        FEET\HUGELAND2.FX  34
	TEnd

	Type		fem
	  Anim		male\shield_WeaponJumpSpin 41
	  PlayFx        FEET\LAND1.FX  34
	TEnd
MEnd

#############################################################

Move A_Shield_Weapon_Jumpspin
	Requires	FLY SHIELD COMBAT WEAPON FIERCE SWORD ATTACK
	Priority	17
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Weapon_Jumpspin_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponJumpSpin 1 40
	TEnd
MEnd

Move A_Shield_Weapon_Jumpspin_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponJumpSpin 41
	TEnd
MEnd


## TAUNT ###########################################################

Move Shield_WeaponTaunt_Draw
	Requires	SHIELD COMBAT WEAPON AXE TAUNT SPAWNBACK
	Priority	69 #This move is 3 higher than it should be
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_WeaponTaunt_Pre
	Sets            CANTMOVE
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd

	Type		huge
	  Anim		huge\draw_axe 1 30
	TEnd

	Type		fem
	  Anim		fem\draw_axe 1 30
	TEnd
MEnd

Move Shield_WeaponTaunt_Pre
	Requires	SHIELD COMBAT WEAPON AXE TAUNT
	Priority	18 #This move is 3 higher than it should be
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_WeaponTaunt
	Sets            CANTMOVE
	Flags		NoSizeScale FIXARMREG

	Type		male
	  Anim		male\shield_WeaponTaunt 1 46
	  PlayFX	feet\MaleTaunt4.fx
	TEnd


	Type		fem
	  Anim		male\shield_WeaponTaunt 1 46
	  PlayFX	feet\FemTaunt5.fx
	TEnd

	Type		Huge
	  Anim		huge\shield_Weapon_Taunt01 1 46
	  PlayFX	feet\MaleTaunt6.fx
	TEnd
MEnd

Move Shield_WeaponTaunt
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_WeaponTaunt_PostCycle
	Flags		FIXARMREG NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_WeaponTaunt 47 54
	TEnd

	Type		huge
	  Anim		huge\shield_Weapon_Taunt01 47 54
	TEnd
MEnd

Move Shield_WeaponTaunt_PostCycle
	Requires	SHIELD COMBAT WEAPON
	Include sequencers\Members\Combat_Shield.inc
	Flags		Cycle FIXARMREG
	NextMove	postReady_back

	Type		male
	  Anim		male\shield_WeaponTaunt 55
	  PlayFX	WEAPONS\AxeTaunt.fx
	TEnd

	Type		huge
	  Anim		huge\shield_Weapon_Taunt01 55
	  PlayFX	WEAPONS\AxeTaunt.fx
	  Scale		0.875
	TEnd
MEnd

#############################################################

Move A_Shield_WeaponTaunt_Draw
	Requires	FLY SHIELD COMBAT WEAPON AXE TAUNT SPAWNBACK
	Priority	70 #This move is 3 higher than it should be
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_WeaponTaunt_Pre
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_WeaponTaunt_Pre
	Requires	FLY SHIELD COMBAT WEAPON AXE TAUNT
	Priority	29 #This move is 3 higher than it should be
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_WeaponTaunt
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim		male\air_shield_Taunt_Weapon 1 45
	  PlayFX	feet\MaleTaunt4.fx
	TEnd

	Type		fem
	  Anim		male\air_shield_Taunt_Weapon 1 45
	  PlayFX	feet\FemTaunt5.fx
	TEnd

	Type		Huge
	  Anim		huge\air_shield_Taunt_Weapon 1 45
	  PlayFX	feet\MaleTaunt6.fx
	TEnd
MEnd

Move A_Shield_WeaponTaunt
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	A_Shield_WeaponTaunt_PostCycle
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\air_shield_Taunt_Weapon 46 54
	TEnd

	Type		Huge
	  Anim		huge\air_shield_Taunt_Weapon 46 54
	TEnd
MEnd

Move A_Shield_WeaponTaunt_PostCycle
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	Flags		Cycle NoSizeScale
	NextMove	Shield_FlyFall

	Type		male
	  Anim		male\air_shield_Taunt_Weapon 55
	  PlayFX	WEAPONS\AxeTaunt.fx
	TEnd

	Type		huge
	  Anim		huge\air_shield_Taunt_Weapon 55
	  PlayFX	WEAPONS\AxeTaunt.fx
	  Scale		0.875
	TEnd
MEnd


## MACE SHOOT ###########################################################

Move Shield_Mace_Shoot_Draw
	Requires	SHIELD COMBAT WEAPON SHOOT SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Mace_Shoot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Mace_Shoot
	Requires	SHIELD COMBAT WEAPON SHOOT
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Mace_Shoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim		male\shield_mace_shoot 1 45
	  PitchAngle	0.5
	TEnd
MEnd

Move Shield_Mace_Shoot_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Blend
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_mace_shoot 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move A_Shield_Mace_Shoot_Draw
	Requires	FLY SHIELD COMBAT WEAPON SHOOT SPAWNBACK
	Priority	76
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Mace_Shoot
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Mace_Shoot
	Requires	FLY SHIELD COMBAT WEAPON SHOOT
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Mace_Shoot_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim		male\air_shield_mace_shoot 1 45
	  PitchAngle	0.5
	TEnd
MEnd

Move A_Shield_Mace_Shoot_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Blend
	Flags		PitchToTarget NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\air_shield_mace_shoot 46 109
	  PitchEnd	65
	  PitchAngle	0.5
	TEnd
MEnd


## MACE SUMMON ###########################################################

Move Shield_MaceSummon_Draw
	Requires	SHIELD COMBAT WEAPON MACESUMMON SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_MaceSummon
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_MaceSummon
	Requires	SHIELD COMBAT WEAPON MACESUMMON
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_MaceSummon_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Interpolate	8

	Type		male
	  Anim		male\shield_macesummon 1 60
	TEnd
MEnd

Move Shield_MaceSummon_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Blend
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_macesummon 61
	TEnd
MEnd

#############################################################

Move A_Shield_MaceSummon_Draw
	Requires	FLY SHIELD COMBAT WEAPON MACESUMMON SPAWNBACK
	Priority	76
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_MaceSummon
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_MaceSummon
	Requires	FLY SHIELD COMBAT WEAPON MACESUMMON
	Priority	25
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_MaceSummon_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_air_macesummon 1 60
	TEnd
MEnd

Move A_Shield_MaceSummon_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Blend
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_air_macesummon 61
	TEnd
MEnd


## THROW SPEAR ###########################################################

Move Shield_Weapon_ThrowSpear_Draw
	Requires	SHIELD COMBAT WEAPON SPEARTHROWLEFT SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Weapon_ThrowSpear_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	SticksOnChild	RIGHTHAND HIDE QUICK ONE

	Type		male
	  Anim		male\shield_WeaponThrowSpear 1 54
	TEnd
MEnd

Move Shield_Weapon_ThrowSpear
	Requires	SHIELD COMBAT WEAPON SPEARTHROWLEFT
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_Weapon_ThrowSpear_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponThrowSpear 1 54
	TEnd
MEnd

Move Shield_Weapon_ThrowSpear_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Blend
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_WeaponThrowSpear 55
	TEnd
MEnd


## SHEILD BASH w\WEAPON ###########################################################

Move Shield_WeaponBash_Draw
	Requires	SHIELD COMBAT WEAPON MAGICTHROW SPAWNBACK
	Priority	65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_WeaponBash_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move Shield_WeaponBash
	Requires	SHIELD COMBAT WEAPON MAGICTHROW
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_WeaponBash_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\shield_frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move Shield_WeaponBash_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatReady_HalfBlend
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\shield_frisbee 31 70
	TEnd
MEnd

#############################################################

#Move A_Shield_WeaponBash
#	Requires	FLY SHIELD COMBAT MAGICTHROW
#	Priority	14
#	Interrupts	<AttackIRQ>
#	Member		<DeathIRQ> <AttackIRQ>
#	NextMove        A_Shield_WeaponBash_Post
#	Sets            CANTMOVE
#	Flags		PitchToTarget NoSizeScale
#
#	Type		male
#	  Anim          male\air_frisbee 1 30
#	  PitchAngle	0.3
#	  PitchStart	11
#	  PitchEnd	23
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_frisbe 1 30
#	  PitchAngle	0.3
#	  PitchStart	11
#	  PitchEnd	23
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_frisbe 1 30
#	  PitchAngle	0.3
#	  PitchStart	11
#	  PitchEnd	23
#	TEnd
#MEnd
#
#Move A_Shield_WeaponBash_Post
#	Requires	FLY SHIELD COMBAT
#	Include sequencers\Members\Combat_Shield_air.inc
#	NextMove        A_Shield_CombatCycle_Half
##	Flags		NoSizeScale
#
#	Type		male
#	  Anim          male\air_frisbee 31
#	TEnd
#
#	Type		huge
#	  Anim          huge\air_frisbe 31
#	TEnd
#
#	Type		fem
#	  Anim          fem\air_frisbe 31
#	TEnd
#MEnd


## SHIELD GRENADE LOB w\WEAPON ###########################################################

Move Shield_WeaponGrenadeLob_Draw
	Requires	SHIELD COMBAT WEAPON GRENADE SPAWNBACK
	Priority        65
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_WeaponGrenadeLob_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	SticksOnChild	RIGHTHAND HIDE QUICK TWO

	Type		male
	  Anim          male\shield_weapongrenadelob 1 60
	TEnd
MEnd

Move Shield_WeaponGrenadeLob
	Requires	SHIELD COMBAT WEAPON GRENADE
	Priority        14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_WeaponGrenadeLob_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	SticksOnChild	RIGHTHAND HIDE QUICK TWO

	Type		male
	  Anim          male\shield_weapongrenadelob 1 60
	TEnd
MEnd

Move Shield_WeaponGrenadeLob_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

## HAMMER ###########################################################

Move Shield_Hammer_Pre
	Requires	SHIELD COMBAT WEAPON CLUB UPPERCUT
	Priority        15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Hammer
	Sets            CANTMOVE
	Flags		NOSIZESCALE FIXARMREG

	Type		male
	  Anim		male\shield_WeaponSwoop 1 25
	TEnd
MEnd

Move Shield_Hammer
	Requires	SHIELD COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Hammer_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.3

	Type		male
	  Anim		male\shield_WeaponSwoop 26 60
	TEnd
MEnd

Move Shield_Hammer_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Half
	Flags		FIXARMREG NOSIZESCALE ReqInputs
	Scale           0.75

	Type		male
	  Anim		male\shield_WeaponSwoop 61
	TEnd
MEnd

#############################################################

Move A_Shield_Hammer_Pre
	Requires	SHIELD FLY COMBAT WEAPON CLUB UPPERCUT
	Priority        26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Hammer
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponSwoop 1 25
	TEnd
MEnd

Move A_Shield_Hammer
	Requires	SHIELD FLY COMBAT WEAPON
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_Hammer_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.3

	Type		male
	  Anim		male\shield_Air_WeaponSwoop 26 60
	TEnd
MEnd

Move A_Shield_Hammer_Post
	Requires	SHIELD FLY COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatFly_Half
	Scale           0.75
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponSwoop 61
	TEnd
MEnd

#############################################################
## TEMPORARY/PRESTIGE WEAPON POWERS
#############################################################

## BATON ###########################################################

Move Shield_Baton_Draw
	Requires	COMBAT WEAPON KNIFE CLUB SPAWNSIDE SHIELD
	Priority        66
	Interrupts	<AttackIRQ> <ShieldReady> <PostMoveCycle>
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Blend
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_WeaponDrawBack 1
	TEnd
MEnd

Move A_Shield_Baton_Draw
	Requires	FLY COMBAT WEAPON KNIFE CLUB SPAWNSIDE SHIELD
	Priority	77
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatReady_Blend
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_Air_WeaponDrawBack 1
	TEnd
MEnd

## BATON ATTACK ###########################################################

Move Shield_Baton_Attack_Draw
	Requires	COMBAT WEAPON ATTACK KNIFE CLUB SHIELD SPAWNSIDE
	Priority	67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Baton_Attack
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		male
	  Anim          male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_Baton_Attack
	Requires	COMBAT WEAPON ATTACK KNIFE CLUB SHIELD
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_Baton_Attack_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_WeaponChop 1 38
	TEnd
MEnd

Move Shield_Baton_Attack_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\shield_WeaponChop 39
	TEnd
MEnd

#############################################################

Move A_Shield_Baton_Attack_Draw
	Requires	FLY COMBAT WEAPON ATTACK KNIFE CLUB SHIELD SPAWNSIDE
	Priority	78
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Baton_Attack
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_Baton_Attack
	Requires	FLY COMBAT WEAPON ATTACK KNIFE CLUB SHIELD
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_Baton_Attack_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponChop 1 40
	TEnd
MEnd

Move A_Shield_Baton_Attack_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponChop 41
	TEnd
MEnd


## GHOST SLAYING AXE ###########################################################

Move Shield_GhostAxe_Draw
	Requires	COMBAT WEAPON GHOSTAXE UPPERCUT SPAWNBACK SHIELD
	Priority	66
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_GhostAxe
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponDrawBack 1 30
	TEnd
MEnd

Move Shield_GhostAxe
	Requires	COMBAT WEAPON GHOSTAXE UPPERCUT SHIELD
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Shield_GhostAxe_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_WeaponChop 1 38
	TEnd
MEnd

Move Shield_GhostAxe_Post
	Requires	COMBAT SHIELD
	Include sequencers\Members\Combat_Shield.inc
	NextMove	Shield_CombatReady_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_WeaponChop 39
	TEnd
MEnd

#############################################################

Move A_Shield_GhostAxe_Draw
	Requires	FLY COMBAT WEAPON GHOSTAXE UPPERCUT SPAWNBACK SHIELD
	Priority	77
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_GhostAxe
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponDrawBack 1 30
	TEnd
MEnd

Move A_Shield_GhostAxe
	Requires	FLY COMBAT WEAPON GHOSTAXE UPPERCUT SHIELD
	Priority	26
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	A_Shield_GhostAxe_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim		male\shield_Air_WeaponChop 1 40
	TEnd
MEnd

Move A_Shield_GhostAxe_Post
	Requires	FLY COMBAT SHIELD
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove	Shield_CombatFly_Half
	Flags		NOSIZESCALE ReqInputs

	Type		male
	  Anim		male\shield_Air_WeaponChop 41
	TEnd
MEnd

## MAGIC STAFF ###########################################################

Move Shield_MagicStaff_Draw
	Requires	SHIELD COMBAT WEAPON CLUB STAFF CAST SPAWNSIDE
	Priority        67
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_MagicStaff
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_firesword_draw 1 25
	TEnd
MEnd

Move Shield_MagicStaff
	Requires	SHIELD COMBAT WEAPON CLUB STAFF CAST
	Priority        16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Shield_MagicStaff_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_staffcast_med 1 54
	TEnd
MEnd

Move Shield_MagicStaff_Post
	Requires	SHIELD COMBAT
	Include sequencers\Members\Combat_Shield.inc
	NextMove        Shield_CombatReady_Half

	Type		male
	  Anim          male\shield_staffcast_med 55
	TEnd
MEnd

#############################################################

Move A_Shield_MagicStaff_Draw
	Requires	FLY SHIELD COMBAT WEAPON CLUB STAFF CAST SPAWNSIDE
	Priority	78
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_MagicStaff
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_air_firesword_draw 1 25
	TEnd
MEnd

Move A_Shield_MagicStaff
	Requires	FLY SHIELD COMBAT WEAPON CLUB STAFF CAST
	Priority	27
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        A_Shield_MagicStaff_Post
	Interpolate     8
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE

	Type		male
	  Anim          male\shield_air_staffcast_med 1 54
	  PitchStart	10
	  PitchEnd	35
	TEnd
MEnd

Move A_Shield_MagicStaff_Post
	Requires	FLY SHIELD COMBAT
	Include sequencers\Members\Combat_Shield_air.inc
	NextMove        Shield_CombatFly_Half
	Flags		NOSIZESCALE

	Type		male
	  Anim          male\shield_air_staffcast_med 55
	TEnd
MEnd