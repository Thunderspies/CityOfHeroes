#############################################################
## Deaths.inc - Deaths
#############################################################

#############################################################
## Default Death
#############################################################

Move Default_Death
	Requires	DEATH
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        Default_postdeath
	Priority        1
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\death_front 47 129
	  PlayFx        FEET\FEMDEATH1.FX
	  Ragdoll	1
	TEnd
MEnd

Move Default_postdeath
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET
	Wind		DEATHWIND

	Type		male
	  Anim          male\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\death_front 129 130
	  Ragdoll	1
	TEnd
MEnd

#############################################################
## Default Death
#############################################################

Move KHF_Deathcycle
	Requires	DEATH
	Member 		<RaiseDeadBackIRQ>
	Interrupts	getup_back Alt_KHF_impact2 Ragdoll_belly
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Interpolate     0
	Sets            NOCOLLISION CANTMOVE DIENOW TARGET

	Type		male
	  Anim          male\knockback_fierce 71 75
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 71 75
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 71 75
	  Ragdoll	1
	TEnd
MEnd

#############################################################
## Impact Death
#############################################################

Move Impact_Death
	Requires	DEATH
	Interrupts	getup_back getup_back2
	NextMove        Impact_postdeath
	Priority        1
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\knockback_fierce 27
	  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 27
	  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 27
	  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd
MEnd

Move Impact_postdeath
	Requires	DEATH
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\knockback_fierce 71 75
	  Ragdoll	0
	TEnd

	Type		huge
	  Anim          huge\knockback_fierce 71 75
	  Ragdoll	0
	TEnd

	Type		fem
	  Anim          fem\knockback_fierce 71 75
	  Ragdoll	0
	TEnd
MEnd


#############################################################
## Vanish Death (instant)
#############################################################

Move Vanish_Death
	Requires	DEATH HIDE
	Interrupts	<DeathIRQ>
	NextMove        Vanish_postdeath
	Priority        5
	Sets            NOCOLLISION
	Flags		NOTSELECTABLE

	Type		male
	  Anim          male\Skel_ready2 1 10
	 PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

Move Vanish_postdeath
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW

	Type		male
	  Anim          male\Skel_ready2 10 15
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd


#############################################################
## SPECIAL ROCK VANISH DEATH
#############################################################

Move R_Vanish_Death
	Requires	DEATH ROCK
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ>
	NextMove        R_Vanish_postdeath
	Priority        6
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\death_front 47 129
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

Move R_Vanish_postdeath
	Requires	DEATH ROCK
	Flags		CYCLE #NOTSELECTABLE
	Priority        6
	Sets            NOCOLLISION DIENOW

	Type		male
	  Anim          male\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd


#############################################################
## Vaporize Death (vanish + freezestun)
#############################################################

Move Vaporize_Death
	Requires	DEATH HIDE FIERCE
	Interrupts	<DeathIRQ>
	NextMove        Vaporize_postdeath
	Priority        7
	Sets            NOCOLLISION
	Flags		NOTSELECTABLE

	Type		male
	  Anim          male\freezestun 1 20
	 PlayFx        GENERIC\ENTITYVANISH.FX  20 CONSTANT
	TEnd
MEnd

Move Vaporize_postdeath
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW


	Type		male
	  Anim          male\freezestun 20 30
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd


#############################################################
## PLAYER_Vaporize Death (vanish + freezestun)
#############################################################

Move PLAYER_Vaporize_Death
	Requires	DEATH HIDE FIERCE PLAYER
	Interrupts	<DeathIRQ>
	NextMove        PLAYER_Vaporize_postdeath
	Priority        8
	Sets            NOCOLLISION
	Flags		NOTSELECTABLE

	Type		male
	  Anim          male\freezestun 1 20
	TEnd
MEnd

Move PLAYER_Vaporize_postdeath
	Requires	DEATH PLAYER
	Member		<RaiseDeadIRQ>
	Flags		CYCLE
	Priority        2
	Sets            NOCOLLISION DIENOW

	Type		male
	  Anim          male\freezestun 20 30
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd


#############################################################
## Hit Death
#############################################################

Move HitDeath
	Requires	HIT DEATH
	Interrupts	<ReactIRQ> <DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        postdeath2
	Priority        2
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\death_front 3 129
	  PlayFx        FEET\MALEDEATH2.FX
	  Ragdoll	5
	TEnd
MEnd

Move postdeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW
	Wind		DEATHWIND

	Type		male
	  Anim          male\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		huge
	  Anim          huge\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\death_front 129 130
	  Ragdoll	1
	TEnd
MEnd


#############################################################
## Electro Death
#############################################################

Move ElectroDeath
	Requires	HIT DEATH ENERGY
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        ElectroDeath2
	Priority        3
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\electro 1
	TEnd

	Type		huge
	  Anim          huge\electro 1
	TEnd

	Type		fem
	  Anim          fem\electro 1 47
	TEnd
MEnd

Move ElectroDeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	NextMove        ElectroDeath3
	Priority        1
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\death_front 53 127
	TEnd

	Type		huge
	  Anim          huge\death_front 53 127
	TEnd

	Type		fem
	  Anim          fem\death_front 53 127
	TEnd
MEnd

Move ElectroDeath3
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\death_front 128 129
	TEnd

	Type		huge
	  Anim          huge\death_front 128 129
	TEnd

	Type		fem
	  Anim          fem\death_front 128 129
	TEnd
MEnd


#############################################################
## Disrupt Death
#############################################################

Move DisruptDeath
	Requires	HIT DEATH DISRUPT
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        DisruptDeath2
	Priority        3
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\electro 1 47
	TEnd

	Type		huge
	  Anim          huge\electro 1 47
	TEnd

	Type		fem
	  Anim          fem\electro 1 47
	TEnd
MEnd

Move DisruptDeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	NextMove        DisruptDeath3
	Priority        1
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\ko 1 100
	TEnd

	Type		huge
	  Anim          huge\ko 1 100
	TEnd

	Type		fem
	  Anim          fem\ko 1 100
	TEnd
MEnd

Move DisruptDeath3
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\ko 100 101
	TEnd

	Type		huge
	  Anim          huge\ko 100 101
	TEnd

	Type		fem
	  Anim          fem\ko 100 101
	TEnd
MEnd


#############################################################
## Choke Death
#############################################################

Move ChokeDeath
	Requires	HIT DEATH MAGIC
	Interrupts	<DeathIRQ> <BasicDeath>
	Member		<RaiseDeadIRQ>
	NextMove        ChokeDeath2
	Priority        3
	Interpolate     8
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\choke 1 137
	TEnd

	Type		huge
	  Anim          huge\choke 1 137
	TEnd

	Type		fem
	  Anim          fem\choke 1 137
	TEnd
MEnd

Move ChokeDeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	NextMove        ChokeDeath3
	Priority        1
	Interpolate     12
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\death_front 46
	TEnd

	Type		huge
	  Anim          huge\death_front 46
	TEnd

	Type		fem
	  Anim          fem\death_front 46
	TEnd
MEnd

Move ChokeDeath3
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\death_front 128 129
	TEnd

	Type		huge
	  Anim          huge\death_front 128 129
	TEnd

	Type		fem
	  Anim          fem\death_front 128 129
	TEnd
MEnd


#############################################################
## Sonic Death
#############################################################

Move SonicDeath
	Requires	HIT DEATH PSIONIC
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        SonicDeath2
	Priority        3
	Interpolate     8
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\sonicpain 1 124
	TEnd

	Type		huge
	  Anim          huge\sonic_pain 1 124
	TEnd

	Type		fem
	  Anim          fem\sonic_pain 1 124
	TEnd
MEnd

Move SonicDeath2
	Requires	DEATH
	NextMove        SonicDeath3
	Priority        1
	Interpolate     12
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\death_front 46
	TEnd

	Type		huge
	  Anim          huge\death_front 46
	TEnd

	Type		fem
	  Anim          fem\death_front 46
	TEnd
MEnd

Move SonicDeath3
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\death_front 128 129
	TEnd

	Type		huge
	  Anim          huge\death_front 128 129
	TEnd

	Type		fem
	  Anim          fem\death_front 128 129
	TEnd
MEnd


#############################################################
## Speed Death
#############################################################

Move SpeedDeath
	Requires	TRAIN DEATH SPEEDPUNCH
	Interrupts	<DeathIRQ> ##weak_post
	NextMove        SpeedDeath2
	Priority        3
	Interpolate     2
	Sets            NOCOLLISION
	#Flags		NOTSELECTABLE

	Type		male
	  Anim          male\knockback 3 38
	  PlayFx        FEET\MaleDeath7.FX  8
	TEnd

	Type		huge
	  Anim          huge\knockdown 3 38
	  PlayFx        FEET\MaleDeath7.FX  8
	TEnd

	Type		fem
	  Anim          fem\knockdown 3 38
	  PlayFx        FEET\MaleDeath7.FX  8
	TEnd

	Type		Sprocket
	  Anim          male\knockback 3 38
	  PlayFx        FEET\clockdeath5.FX  8
	TEnd
MEnd

Move SpeedDeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		CYCLE #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\knockback 38 40
	TEnd

	Type		huge
	  Anim          huge\knockdown 38 40
	TEnd

	Type		fem
	  Anim          fem\knockdown 38 40
	TEnd
MEnd


#############################################################
## Pose Death (gang wars)
#############################################################

Move PoseDeath		##(thugs-pose run away)
	Requires	Hit DEATH POSEDEATH
	Interrupts	<DeathIRQ>
	NextMove	PoseDeath_cycle
	Priority        3
	Interpolate     8
	Flags		NOCOLLISION #NOTSELECTABLE

	Type		male
	  Anim          male\run_away 3
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd
MEnd

Move PoseDeath_cycle
	Requires	DEATH
	Flags		CYCLE NOCOLLISION #NOTSELECTABLE
	Priority        1
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\run_away 3
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd
MEnd