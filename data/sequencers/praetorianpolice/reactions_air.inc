#############################################################
## reactions_air.inc - Hits, Deaths, Rez, Status Effects, etc.
#############################################################


###################
###### Air Hit Quick  ######
###################

Move A_HitQuick
	Requires	FLY QUICK HIT
	Priority        3
	Interrupts	<HitIRQ> A_FearStun_cycle A_FearStun
	Include sequencers\Members\Reactions_Air.inc
	NextMove        C_flycycle
	Flags		HitReact

	Type		male
	  Anim          male\air_combat_HQ 2
	TEnd
MEnd

###################
###### Air Combat Hit Quick  ######
###################

Move A_C_HitQuick
	Requires	FLY COMBAT QUICK HIT
	Priority        4
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        C_flycycle
	Flags		HitReact

	Type		male
	  Anim          male\air_combat_HQ 2
	TEnd
MEnd

#############################################################

Move A_C_HitQuick_L
	Requires	FLY COMBAT QUICK HIT
	Priority        4
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        C_flycycle
	Flags		HitReact

	Type		male
	  Anim          male\air_combat_HQ 2
	TEnd
MEnd

###################
###### Air Weapon Hit Quick  ######
###################

Move A_W_HitQuick
	Requires	FLY COMBAT WEAPON QUICK HIT
	Priority        6
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        W_flycycle
	Flags		HitReact

	Type		male
	  Anim          male\air_wep_HQ 1
	TEnd
MEnd

###################
###### Air cage react\wriggle  ######
###################

Move A_cage_wriggle
	Requires	FLY HIT WRIGGLE
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	Flags		ReqInputs
	NextMove        A_cage_wriggle2
	Scale           0.60

	Type		male
	  Anim          male\air_wriggle 1 17
	TEnd
MEnd

Move A_cage_wriggle2
	Requires	FLY HIT WRIGGLE
	Include sequencers\Members\Reactions_Air.inc
	Flags		Cycle
	NextMove        flyready
	Scale           0.60

	Type		male
	  Anim          male\air_wriggle 18
	TEnd
MEnd

###################
###### Air electro\pain react  ######
###################

Move A_electro_react
	Requires	FLY HIT ENERGY
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_weak_post
	Flags		HitReact

	Type		male
	  Anim          male\air_electro 1
	TEnd
MEnd

###################
###### Air weak post  ######
###################

Move A_weak_post
	Requires	FLY
	Include sequencers\Members\Readies_Air.inc
	NextMove        flyready

	Type		male
	  Anim          male\air_drain 40
	TEnd
MEnd


###################
###### Air drain react  ######
###################

Move A_Drain_react
	Requires	FLY HIT DRAINED
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        flyready
	Flags		HitReact

	Type		male
	  Anim          male\air_drain 1
	TEnd
MEnd


###################
###### Air sonic\psi pain react  ######
###################

Move A_sonic_react
	Requires	FLY HIT PSIONIC
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_weak_post

	Type		male
	  Anim          male\air_sonicpain 1
	TEnd
MEnd


###################
###### Air choke react  ######
###################

Move A_choke_react
	Requires	FLY HIT MAGIC
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_weak_post
	Flags		HitReact

	Type		male
	  Anim          male\air_choke 1
	TEnd
MEnd


###################
###### Air Speed_attack react  ######
###################

Move A_Speed_react
	Requires	FLY HIT SPEEDPUNCH
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_weak_post
	Flags		HitReact

	Type		male
	  Anim          male\air_speedreact 3
	TEnd
MEnd


###################
###### Air Kungfu_speed react  ######
###################

Move A_KungfuSpeed_react
	Requires	FLY HIT KUNGFUSPEED
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_weak_post
	Flags		HitReact

	Type		male
	  Anim          male\air_speedreact 3
	TEnd
MEnd

## AIR CLAWS SPEED REACT ###########################################################

Move A_ClawsSpeed_react
	Requires	FLY HIT CLAWSPEED
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_weak_post
	Flags		HitReact

	Type		male
	  Anim          male\air_speedreact 3
	TEnd
MEnd


#############################################################
## Dual Blades Speed Hit Reaction
#############################################################

Move A_DualBlade_SpeedHit
	Requires	FLY HIT DUAL CLAWSPEED
	Priority        3
	Interrupts	<HitIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_weak_post
	Flags		HitReact

	Type		male
	  Anim          male\air_speedreact 3
	TEnd
MEnd


##################
###### Air Boost_up react  ######
###################

Move A_Hit_Boostup
	Requires	FLY HIT BOOSTUP
	Priority        3
	Flags		HitReact
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        C_flycycle

	Type		male
	  Anim          male\air_boostup 1
	TEnd
MEnd

#############################################################

Move A_hit_Boost_D
	Requires	FLY HIT BOOSTDOWN
	Priority        3
	Interrupts	<ReactIRQ>
	Include sequencers\Members\Reactions_Air.inc
	NextMove        C_flycycle
	Flags		HitReact

	Type		male
	  Anim          male\air_boostdown 1
	TEnd
MEnd


###################
###### Air Fear Stun  ######
###################

Move A_FearStun
	Requires	FLY FEAR
	Priority        3
	Interrupts	<ReactIRQ> Stun Stun_cycle
	Include sequencers\Members\Combat_Air.inc
	NextMove        A_FearStun_cycle

	Type		male
	  Anim          male\air_fear_stun 1
	TEnd
MEnd

Move A_FearStun_cycle
	Requires	FLY FEAR
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle
	Interpolate	8

	Type		male
	  Anim          male\air_fear_stun 25 60
	TEnd
MEnd


###################
###### Air knockback Hit  ######
###################

Move A_KHF
	Requires	FLY HIT KNOCKBACK
	Priority        8
	Interrupts	<HitIRQ> <DeathIRQ> KHF
	NextMove        A_KHF_Post
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_KB_quick 1 55
	TEnd
MEnd

Move A_KHF_Post
	Requires	FLY
	Include sequencers\Members\Reactions_Air.inc
	NextMove        A_KHF_PostCycle
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_KB_quick 55
	TEnd
MEnd

Move A_KHF_PostCycle
	Requires	FLY
	Include sequencers\Members\Reactions_Air.inc
	NextMove        flyready
	Flags		NoSizeScale CYCLE

	Type		male
	  Anim		male\shield_CombatFly 1 31
	TEnd
MEnd

###################
###### Air Sweeps  ######
###################

Move A_Sweep_low
	Requires	SWEEP LOW HIT FLY #PLAYER
	Priority        5
	Interrupts	<HitIRQ> <DeathIRQ> Sweep_low
	NextMove        flyready
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_KB_quick 1
	TEnd
MEnd

#############################################################

Move A_Sweep_High
	Requires	SWEEP HIGH HIT FLY #PLAYER
	Priority        5
	Interrupts	<HitIRQ> <DeathIRQ> Sweep_Highk
	NextMove        flyready
	Sets		CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\air_KB_quick 1
	TEnd
MEnd