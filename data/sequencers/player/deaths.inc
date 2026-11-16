#############################################################
## Deaths.inc - Deaths
#############################################################

Move KHF_Deathcycle
	Requires	DEATH
	Priority        101
	Member 		<RaiseDeadBackIRQ>
	Interrupts	getup_back Alt_KHF_impact2 Ragdoll_belly
	Flags		Cycle
	Interpolate     0
	NextMove	Revive_belly_Pre_rooted
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
	Priority        101
	Interrupts	getup_back getup_back2
	NextMove        Impact_postdeath
	Sets            NOCOLLISION

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


	Type		Sprocket
	  Anim          male\knockback_fierce 27
	  PlayFx        FEET\clockdeath1.FX
	  Ragdoll	0
	TEnd

	Type		P_AntiMatter
	   Anim         male\knockback_fierce 27
	   PlayFX	Events\EndGame\KeyesIsland\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		V_Shalice
	   Anim         fem\knockback_fierce 27
	   PlayFX	Events\EndGame\KeyesIsland\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\knockback_fierce 27
	  #PlayFx        FEET\MALEDEATH4.FX
	  #Ragdoll	0
	TEnd

	Type		P_Apparitions_Minion
	  Anim          fem\knockback_fierce 27
	#  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\knockback_fierce 27
	#  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          fem\knockback_fierce 27
	#  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\knockback_fierce 27
	#  PlayFx        FEET\MALEDEATH4.FX
	  Ragdoll	0
	TEnd

	Type		V_Summer_Apocalypse
	   Anim         male\knockback_fierce 27
	   PlayFX	Events\Summer\Blue\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		V_Summer_Armageddon
	   Anim         male\knockback_fierce 27
	   PlayFX	Events\EndGame\KeyesIsland\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		V_Summer_Gotterdammerung
	   Anim         huge\knockback_fierce 27
	   PlayFX	Events\Summer\Grey\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

MEnd

Move Impact_postdeath
	Requires	DEATH
	Member 		<RaiseDeadBackIRQ>
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
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
	Priority        102
	Interrupts	<DeathIRQ>
	NextMove        Vanish_postdeath
	Sets            NOCOLLISION
	Flags		NotSelectable

	Type		male
	  Anim          male\Skel_ready2 1 10
	 PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move Vanish_postdeath
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW

	Type		male
	  Anim          male\Skel_ready2 10 15
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd


#############################################################
## SPECIAL ROCK VANISH DEATH
#############################################################

Move R_Vanish_Death
	Requires	DEATH ROCK
	Priority        102
	Interrupts	<DeathIRQ>
	Member		<ExplodeIRQ>
	NextMove        R_Vanish_postdeath
	Sets            NOCOLLISION
	Flags		NotSelectable

	Type		male
	  Anim          male\death_front 47 129
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd

Move R_Vanish_postdeath
	Requires	DEATH
	Flags		Cycle NotSelectable
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
	Priority        103
	Interrupts	<DeathIRQ>
	NextMove        Vaporize_postdeath
	Sets            NOCOLLISION
	Flags		NotSelectable

	Type		male
	  Anim          male\freezestun 1 20
	 PlayFx        GENERIC\ENTITYVANISH.FX  20 CONSTANT
	TEnd
MEnd

Move Vaporize_postdeath
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle NotSelectable
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
	Priority        104
	Interrupts	<DeathIRQ>
	NextMove        PLAYER_Vaporize_postdeath
	Flags		NotSelectable
	Sets            NOCOLLISION

	Type		male
	  Anim          male\freezestun 1 20
##	  PlayFX	GENERIC\pumpkinspawn.FX  0 CONSTANT
	TEnd
MEnd

Move PLAYER_Vaporize_postdeath
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW

	Type		male
	  Anim          male\freezestun 20 30
##	  PlayFX	GENERIC\pumpkinspawn.FX  0 CONSTANT
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd


## DEMUTATE (w/FX) ###########################################################

Move Demutate
	Requires	DEATH POISON POSEDEATH
	Priority        103
	Interrupts	<DeathIRQ>
	#Member		<RaiseDeadIRQ> - This cannot be interrupted by any animation
	NextMove        Demutate_PostCycle
	Sets            NOCOLLISION DIENOW
	Flags		NotSelectable

	Type		male
	  Anim		male\freezestun 1 20
	  PlayFx	GENERIC\Demutate.fx 10 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\freeze 1 20
	  PlayFx	GENERIC\Demutate.fx 10 CONSTANT
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim		male\freezestun 1 20
	  #PlayFx	GENERIC\Demutate.fx 10 CONSTANT
	TEnd

	Type		P_Apparitions_Minion
	  Anim		male\freezestun 1 20
	  #PlayFx	GENERIC\Demutate.fx 10 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		male\freezestun 1 20
	  #PlayFx	GENERIC\Demutate.fx 10 CONSTANT
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim		male\freezestun 1 20
	  #PlayFx	GENERIC\Demutate.fx 10 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim		male\freezestun 1 20
	  #PlayFx	GENERIC\Demutate.fx 10 CONSTANT
	TEnd

MEnd

Move Demutate_PostCycle
	#Member		<RaiseDeadIRQ> - This cannot be interrupted by any animation
	Flags		Cycle NotSelectable
	Sets            NOCOLLISION DIENOW

	Type		male
	  Anim          male\freezestun 20 30
	  PlayFx	GENERIC\Demutate.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\freeze 20 30
	  PlayFx	GENERIC\Demutate.fx 0 CONSTANT
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\freezestun 20 30
	  #PlayFx	GENERIC\Demutate.fx 0 CONSTANT
	TEnd

	Type		P_Apparitions_Minion
	  Anim          male\freezestun 20 30
	 # PlayFx	GENERIC\Demutate.fx 0 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\freezestun 20 30
	 # PlayFx	GENERIC\Demutate.fx 0 CONSTANT
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          male\freezestun 20 30
	 # PlayFx	GENERIC\Demutate.fx 0 CONSTANT
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\freezestun 20 30
	 # PlayFx	GENERIC\Demutate.fx 0 CONSTANT
	TEnd

MEnd


#############################################################
## Default Death - Player
#############################################################

Move Player_Default_Death
	Requires	PLAYER DEATH
	Priority        102
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        Default_postdeath
	Sets            NOCOLLISION

	Type		male
	  Anim          male\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		huge
	  Anim          huge\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		fem
	  Anim          fem\death_front 47 129
	  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\death_front 47 129
	 # PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		P_Apparitions_Minion
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		monkey_man_endgame
	  Anim          male\death_front 47 129
	TEnd



MEnd

#############################################################
## Default Death
#############################################################

Move Default_Death
	Requires	DEATH
	Priority        101
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        Default_postdeath
	Sets            NOCOLLISION

	Type		male
	  Anim          male\death_front 47 129
	  PlayFx        Feet\NPCMaleDeath1.fx
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		huge
	  Anim          huge\death_front 47 129
	   PlayFx        FEET\MALEDEATH1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		fem
	  Anim          fem\death_front 47 129
	  PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		sprocket
	  Anim          male\death_front 47 129
	  PlayFx        FEET\clockdeath4.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		Carnival_masks
	  Anim          fem\carnival_death 1 181
	  PlayFx        FEET\COSFEMSCREAM.FX
	TEnd

	Type		fem_floater
	  Anim          fem\carnival_death 1 181
	  PlayFx        FEET\NPCFEMSCREAM.FX
	TEnd

	Type		male_teledeath
	  Anim          male\absorb 1
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_RecluseEscape.fx 0 Constant
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_death 1
	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 123 Constant
	  PlayFx        POWERS\Ability\StrengthHands2.fx 123 Constant
	  PlayFx        ENEMYFX\Hydra\Hydra_Head_Death\Rularuu_Death.fx 0 #155
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 1 20
	TEnd

	Type		male_floater_Pirate
	  Anim          male\air_drain 1
	TEnd

	Type		Statesman_Escape
	  Anim          male\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
#	  Ragdoll	1
	TEnd

	Type		Liberty_Escape
	  Anim          fem\death_front 47 129
	  PlayFx        FEET\NPCFemDeath1.fx
#	  Ragdoll	1
	TEnd

	Type		Statesman
	  Anim          male\death_front 47 129
	  PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	1
	TEnd

	Type		LordRecluse
	  Anim          male\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	  Ragdoll	1
	TEnd

	Type		LordRecluse_Escape
	  Anim          male\wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd

	Type		Romulus
	  Anim          male\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	  #Ragdoll	1
	TEnd

	Type		V_Shalice
	   Anim         male\mystic_death 1
	   PlayFX	ENEMYFX\Praetorian\MoM\Shalice_Defeat.fx
	  # Ragdoll	30
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\death_front 47 129
	  #PlayFx        Feet\NPCMaleDeath1.fx
	 # Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_Apparitions_Minion
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		Abomination
	  Anim          huge\death_front 47 129
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Death.fx 0 CONSTANT
	Scale		0.80
	TEnd

	Type		monkey_man_endgame
	  Anim          male\death_front 47 129
	TEnd

	Type		MaleFallForward
	  Anim          male\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		HugeFallForward
	  Anim          huge\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		FemFallForward
	  Anim          fem\death_front 47 129
	  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		male_death_rest
	  Anim          male\rest_kneel 1 50
	TEnd

	Type		fem_death_rest
	  Anim          male\rest_kneel 1 50
	TEnd

	Type		huge_death_rest
	  Anim          male\rest_kneel 1 50
	TEnd


MEnd

Move Default_postdeath
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
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

	Type		Carnival_masks
	  Anim          fem\carnival_death 181 185
	TEnd

	Type		fem_floater
	  Anim          fem\carnival_death 181 185
	TEnd

	Type		male_teledeath
	  Anim          male\Skel_ready2 1
	  PlayFx        V_COV\EnemyPowers\Guards\F_FadeOut_RecluseEscape.fx 0 Constant
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 20 30
	TEnd

	Type		male_floater_Pirate
	  Anim          male\air_drain 63
	TEnd

	Type		Statesman_Escape
	  Anim          male\death_front 129 130
#	  Ragdoll	1
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_Entity.fx 0 Constant
	  PlayFX	V_COV\EnemyPowers\Guards\FX_StatesmanGuard.fx 0 Constant
	TEnd

	Type		Liberty_Escape
	  Anim          fem\death_front 129 130
#	  Ragdoll	1
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_Entity.fx 0 Constant
	  PlayFX	V_COV\EnemyPowers\Guards\FX_StatesmanGuard.fx 0 Constant
	TEnd

	Type		Statesman
	  Anim          male\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		LordRecluse
	  Anim          male\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		LordRecluse_Escape
	  Anim          male\absorb 1
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_RecluseEscape.fx 0 Constant
	TEnd

	Type		Romulus
	  Anim          male\death_front 129 130
	  PlayFx        FEET\MALEDEATH1.FX
	  #Ragdoll	1
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\death_front 129 130
	 # Ragdoll	1
	TEnd

	Type		P_Apparitions_Minion
 	 Anim          fem\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		P_Apparitions_Lieutenant
 	 Anim          fem\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		P_Apparitions_Minion_Merged
 	 Anim          fem\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		P_Apparitions_Lieutenant
 	 Anim          fem\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		Abomination
	  Anim          huge\death_twitch 1
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Death_Twitch.fx #0 CONSTANT
	TEnd

	Type		monkey_man_endgame
	  Anim          male\death_front 47 129
	  Ragdoll	0
	TEnd

	Type		male_death_rest
	  Anim          male\rest_kneel 50 60
	TEnd

	Type		fem_death_rest
	  Anim          male\rest_kneel 50 60
	TEnd

	Type		huge_death_rest
	  Anim          male\rest_kneel 50 60
	TEnd

MEnd


#############################################################
## MW changed GhostTrapDeath Death (special one off death for ghost)
#############################################################

Move GhostTrapDeath
	Requires	DEATH HEADPAIN
	Priority        102
	Interrupts	<DeathIRQ> <RaiseDeadIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        Default_postdeath
	Sets            NOCOLLISION

	Type		male
	  Anim          male\death_front 47 129
	  PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		male_floater_Pirate
	  Anim          male\air_electro 1
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\death_front 47 129
	 # PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		P_Apparitions_Minion
	  Anim          male\death_front 47 129
	# PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\death_front 47 129
	# PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          male\death_front 47 129
	# PlayFx        FEET\MALEDEATH1.FX
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\death_front 47 129
	# PlayFx        FEET\MALEDEATH1.FX
	TEnd

MEnd


#############################################################
## Hit Death - Players only
#############################################################

Move Player_HitDeath
	Requires	PLAYER HIT DEATH
	Priority        103
	Interrupts	<DeathIRQ> <HitIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        postdeath2
	Sets            NOCOLLISION

	Type		male
	  Anim          male\death_front 3 129
	  PlayFx        FEET\MALEDEATH2.FX
	TEnd

	Type		huge
	  Anim          huge\death_front 47 129
	  PlayFx        FEET\HUGEDEATH2.FX
	TEnd

	Type		fem
	  Anim          fem\death_front 47 129
	  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\death_front 3 129
	  #PlayFx        FEET\MALEDEATH2.FX
	TEnd

	Type		P_Apparitions_Minion
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	#  PlayFx        FEET\FEMDEATH1.FX
	TEnd

MEnd

#############################################################
## Hit Death
#############################################################

Move HitDeath
	Requires	HIT DEATH
	Priority        102
	Interrupts	<DeathIRQ> <HitIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        postdeath2
	Sets            NOCOLLISION

	Type		male
	  Anim          male\death_front 1 129
	  PlayFx        Feet\NPCMaleDeath1.fx
	  Ragdoll	10 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		huge
	  Anim          huge\death_front 47 129
	  PlayFx        FEET\NPCHugeDeath2.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		fem
	  Anim          fem\death_front 47 129
	  PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		Animus
	  Anim          male\death_front 1 129
	  #PlayFx        Feet\NPCMaleDeath1.fx
	  #Ragdoll	10 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		sprocket
	  Anim          male\death_front 3 129
	  PlayFx        FEET\clockdeath3.FX
	  Ragdoll	13 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		Carnival_masks
	  Anim          fem\carnival_death 1 181
	  PlayFx        FEET\COSFEMSCREAM.FX
	TEnd

	Type		fem_floater
	  Anim          fem\carnival_death 1 181
	  PlayFx        FEET\NPCFEMSCREAM.FX
	TEnd

	Type		male_teledeath
	  Anim          male\absorb 1
	PlayFX		V_COV\EnemyPowers\Guards\F_FadeOut_RecluseEscape.fx 0 Constant
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_death 1
	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 123 Constant
	  PlayFx        POWERS\Ability\StrengthHands2.fx 123 Constant
	  PlayFx        ENEMYFX\Hydra\Hydra_Head_Death\Rularuu_Death.fx 155
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 1 20
	TEnd

	Type		male_floater_Pirate
	  Anim          male\air_drain 1
	TEnd

	Type		Statesman_Escape
	  Anim          male\death_front 3 129
	  PlayFx        FEET\MALEDEATH2.FX
#	  Ragdoll	5
	TEnd

	Type		Liberty_Escape
	  Anim          fem\death_front 3 129
	  PlayFx        FEET\MALEDEATH2.FX
#	  Ragdoll	5
	TEnd

	Type		Statesman
	  Anim          male\death_front 3 129
	  PlayFx        FEET\MALEDEATH2.FX
	  Ragdoll	5
	TEnd

	Type		LordRecluse
	  Anim          male\death_front 3 129
	  PlayFx        FEET\MALEDEATH2.FX
	  Ragdoll	5
	TEnd

	Type		LordRecluse_Escape
	  Anim          male\wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd

	Type		Romulus
	  Anim          male\death_front 3 129
	  PlayFx        FEET\MALEDEATH2.FX
	 # Ragdoll	5
	TEnd

	Type		Pet_ElectricGremlin
	   Anim          male\death_front 1 129
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\death_front 1 129
	  #PlayFx        Feet\NPCMaleDeath1.fx
	  #Ragdoll	10 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_AntiMatter
	   Anim         male\absorb 1 30
	   PlayFX	Events\EndGame\KeyesIsland\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		V_Shalice
	   Anim         male\mystic_death 1
	   PlayFX	ENEMYFX\Praetorian\MoM\Shalice_Defeat.fx
	  # Ragdoll	30
	TEnd

	Type		P_Apparitions_Minion
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\death_front 47 129
	 # PlayFx        FEET\NPCFemDeath1.FX
	  Ragdoll	57 #ragdoll frame is absolute to anim frame -delayed 10 frames to allow AUDIO to play.
	TEnd

	Type		Abomination
	  Anim          huge\death_front 47 129
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Death.fx 0 CONSTANT
	Scale		0.80
	TEnd

	Type		MindsOfMayhem_JackInIrons
	  Anim          huge\electro 1
	  PlayFx        FEET\NPCHugeDeath2.FX
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		V_Summer_Apocalypse
	   Anim         male\electro 1
	   PlayFX	Events\Summer\Blue\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		V_Summer_Armageddon
	   Anim         male\electro 1
	   PlayFX	Events\EndGame\KeyesIsland\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		V_Summer_Gotterdammerung
	   Anim         huge\electro 1
	   PlayFX	Events\Summer\Grey\NuclearExplosion.fx
	   Ragdoll	0
	TEnd

	Type		male_death_rest
	  Anim          male\rest_kneel 1 50
	TEnd

	Type		fem_death_rest
	  Anim          male\rest_kneel 1 50
	TEnd

	Type		huge_death_rest
	  Anim          male\rest_kneel 1 50
	TEnd
MEnd

Move postdeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
	Sets            NOCOLLISION DIENOW
	Wind		DEATHWIND

	Type		male
	  Anim          male\death_front 129 130
	  Ragdoll	5
	TEnd

	Type		huge
	  Anim          huge\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		fem
	  Anim          fem\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		Carnival_masks
	  Anim          fem\carnival_death 181 185
	TEnd

	Type		fem_floater
	  Anim          fem\carnival_death 181 185
	TEnd

	Type		male_teledeath
	  Anim          male\Skel_ready2 1
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_RecluseEscape.fx 0 Constant
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 20 30
	TEnd

	Type		male_floater_Pirate
	  Anim          male\air_drain 63
	TEnd

	Type		Statesman_Escape
	  Anim          male\death_front 129 130
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_Entity.fx 0 Constant
	  PlayFX	V_COV\EnemyPowers\Guards\FX_StatesmanGuard.fx 0 Constant
	TEnd

	Type		Liberty_Escape
	  Anim          fem\death_front 129 130
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_Entity.fx 0 Constant
	  PlayFX	V_COV\EnemyPowers\Guards\FX_StatesmanGuard.fx 0 Constant
	TEnd

	Type		Statesman
	  Anim          male\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		LordRecluse
	  Anim          male\death_front 129 130
	  Ragdoll	1
	TEnd

	Type		LordRecluse_Escape
	  Anim          male\absorb 1
	  PlayFX	V_COV\EnemyPowers\Guards\F_FadeOut_RecluseEscape.fx 0 Constant
	TEnd

	Type		Romulus
	  Anim          male\death_front 129 130
	TEnd

	Type		Pet_ElectricGremlin
	  Anim          male\death_front 129 130
	TEnd

	Type		P_AntiMatter
	  Anim          male\death_front 129 130
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	  Ragdoll	0
	TEnd

	Type		Abomination
	  Anim          huge\death_twitch 1
	  PlayFX	ENEMYFX\Unique\Abomination\Abomination_Death_Twitch.fx #0 CONSTANT
	TEnd

	Type		MindsOfMayhem_JackInIrons
	  Anim          huge\electro 1
	TEnd

	Type		V_Summer_Apocalypse
	   Anim         male\electro 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	  Ragdoll	0
	TEnd

	Type		V_Summer_Armageddon
	   Anim         male\electro 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	  Ragdoll	0
	TEnd

	Type		V_Summer_Gotterdammerung
	   Anim         huge\electro 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	  Ragdoll	0
	TEnd

	Type		male_death_rest
	  Anim          male\rest_kneel 50 60
	TEnd

	Type		fem_death_rest
	  Anim          male\rest_kneel 50 60
	TEnd

	Type		huge_death_rest
	  Anim          male\rest_kneel 50 60
	TEnd

MEnd

#############################################################
## Electro Death
#############################################################

Move ElectroDeath
	Requires	HIT DEATH ENERGY
	Priority        103
	Interrupts	<DeathIRQ> <HitIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        ElectroDeath2
	Sets            NOCOLLISION

	Type		male
	  Anim          male\electro 1
	TEnd

	Type		huge
	  Anim          huge\electro 1
	TEnd

	Type		fem
	  Anim          fem\electro 1 47
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_death 1
	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 123 Constant
	  PlayFx        POWERS\Ability\StrengthHands2.fx 123 Constant
	  PlayFx        ENEMYFX\Hydra\Hydra_Head_Death\Rularuu_Death.fx 155
	  PlayFx        GENERIC\ENTITYVANISH.FX 170 CONSTANT
	TEnd
MEnd

Move ElectroDeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	NextMove        ElectroDeath3
	Sets            NOCOLLISION

	Type		male
	  Anim          male\death_front 53 127
	TEnd

	Type		huge
	  Anim          huge\death_front 53 127
	TEnd

	Type		fem
	  Anim          fem\death_front 53 127
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd

Move ElectroDeath3
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
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


	Type		Rularuu_male
	  Anim          male\mystic_float 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd
MEnd


#############################################################
## Disrupt Death
#############################################################

Move DisruptDeath
	Requires	HIT DEATH DISRUPT
	Priority        103
	Interrupts	<DeathIRQ> <HitIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        DisruptDeath2
	Sets            NOCOLLISION

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
	Sets            NOCOLLISION

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
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
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
	Priority        103
	Interrupts	<DeathIRQ> <HitIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        ChokeDeath2
	Interpolate     8
	Sets            NOCOLLISION

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
	Interpolate     12
	Sets            NOCOLLISION

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
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
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
	Priority        103
	Interrupts	<DeathIRQ> <HitIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        SonicDeath2
	Interpolate     8
	Sets            NOCOLLISION

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
	Interpolate     12
	Sets            NOCOLLISION

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
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
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
	Priority        103
	Interrupts	<DeathIRQ> ##weak_post
	NextMove        SpeedDeath2
	Interpolate     2
	Sets            NOCOLLISION

	Type		male
	  Anim          male\knockback 3 39
	  PlayFx        FEET\MaleDeath7.FX  8
	TEnd

	Type		huge
	  Anim          huge\knockdown 3 39
	  PlayFx        FEET\MaleDeath7.FX  8
	TEnd

	Type		fem
	  Anim          fem\knockdown 3 39
	  PlayFx        FEET\MaleDeath7.FX  8
	TEnd

	Type		Sprocket
	  Anim          male\knockback 3 39
	  PlayFx        FEET\clockdeath5.FX  8
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\knockback 3 39
	TEnd

	Type		P_Apparitions_Minion
	  Anim          fem\knockdown 3 39
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\knockdown 3 39
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          fem\knockdown 3 39
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          fem\knockdown 3 39
	TEnd


MEnd

Move SpeedDeath2
	Requires	DEATH
	Member		<RaiseDeadIRQ>
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\knockback 39 40
	TEnd

	Type		huge
	  Anim          huge\knockdown 39 40
	TEnd

	Type		fem
	  Anim          fem\knockdown 39 40
	TEnd
MEnd


#############################################################
## Pose Death (gang wars)
#############################################################

Move PoseDeath		##(thugs-pose run away)
	Requires	HIT DEATH POSEDEATH
	Priority        103
	Interrupts	<DeathIRQ> <HitIRQ>
	NextMove	PoseDeath_cycle
	Interpolate     8
	Flags		NotSelectable NOCOLLISION

	Type		male
	  Anim          male\run_away 3
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd
MEnd

Move PoseDeath_cycle
	Requires	DEATH
	Flags		Cycle NotSelectable NOCOLLISION
	NextMove	Revive_belly_Pre_rooted
	Sets            NOCOLLISION DIENOW TARGET

	Type		male
	  Anim          male\run_away 3
	  PlayFx	Generic\Faerie_entityFadeOut.fx 0 constant
	TEnd

MEnd

###################
###### Default air Death   ######
###################

Move Air_Death
	Requires	FLY DEATH
	Priority        112
	Interrupts	<DeathIRQ>
	NextMove        Air_Death2

	Type		male
	  Anim          male\air_death 6 7
	TEnd

	Type		huge
	  Anim          huge\air_death 6 7
	TEnd

	Type		fem
	  Anim          fem\air_death 6 7
	TEnd

	Type		male_teledeath
	  Anim          male\air_boostup 1 2
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_death 1 2
	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 123 Constant
	  PlayFx        POWERS\Ability\StrengthHands2.fx 123 Constant
	TEnd

	Type		MindsOfMayhem_Lanaru
	  Anim          male\mystic_death 1 2
#	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
#	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 123 Constant
#	  PlayFx        POWERS\Ability\StrengthHands2.fx 123 Constant
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_Ruladak
	  Anim          male\mystic_death 1 2
#	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
#	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 123 Constant
#	  PlayFx        POWERS\Ability\StrengthHands2.fx 123 Constant
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		MindsOfMayhem_Rularuu
	  Anim          male\mystic_death 1 2
#	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
#	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 123 Constant
#	  PlayFx        POWERS\Ability\StrengthHands2.fx 123 Constant
	  PlayFx        ENEMYFX\Praetorian\MoM\Malaise_Nightmare_Death.fx
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 1 20
	TEnd
MEnd

Move Air_Death2
	Requires	AIR DEATH
##	Flags		ReqInputs
	NextMove        Air_death_fall

	Type		male
	  Anim          male\air_death 8 51
	TEnd

	Type		huge
	  Anim          huge\air_death 8 51
	TEnd

	Type		fem
	  Anim          fem\air_death 8 51
	TEnd

	Type		male_teledeath
	  Anim          male\air_boostup 3 70
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_death 3
	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 0 Constant
	  PlayFx        POWERS\Ability\StrengthHands2.fx 0 Constant
	  PlayFx        ENEMYFX\Hydra\Hydra_Head_Death\Rularuu_Death.fx 153
	  PlayFx        GENERIC\ENTITYVANISH.FX 168 CONSTANT
	TEnd

	Type		MindsOfMayhem_Lanaru
	  Anim          male\mystic_death 3
#	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
#	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 0 Constant
#	  PlayFx        POWERS\Ability\StrengthHands2.fx 0 Constant
#	  PlayFx        ENEMYFX\Hydra\Hydra_Head_Death\Rularuu_Death.fx 153
#	  PlayFx        GENERIC\ENTITYVANISH.FX 168 CONSTANT
	TEnd

	Type		MindsOfMayhem_Ruladak
	  Anim          male\mystic_death 3
#	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
#	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 0 Constant
#	  PlayFx        POWERS\Ability\StrengthHands2.fx 0 Constant
#	  PlayFx        ENEMYFX\Hydra\Hydra_Head_Death\Rularuu_Death.fx 153
#	  PlayFx        GENERIC\ENTITYVANISH.FX 168 CONSTANT
	TEnd

	Type		MindsOfMayhem_Rularuu
	  Anim          male\mystic_death 3
#	  PlayFx        POWERS\Ability\AgilityContinuing.fx 0 Constant
#	  PlayFx        POWERS\Ability\WillpowerContinuingRays.fx 0 Constant
#	  PlayFx        POWERS\Ability\StrengthHands2.fx 0 Constant
#	  PlayFx        ENEMYFX\Hydra\Hydra_Head_Death\Rularuu_Death.fx 153
#	  PlayFx        GENERIC\ENTITYVANISH.FX 168 CONSTANT
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 20 30
	TEnd
MEnd

Move Air_death_fall
	Requires	AIR DEATH
	Flags		Cycle
	NextMove        Air_death_impact

	Type		male
	  Anim          male\air_death 53 78
	TEnd

	Type		huge
	  Anim          huge\air_death 53 78
	TEnd

	Type		fem
	  Anim          fem\air_death 53 78
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 20 30
	TEnd
MEnd

Move Air_death_impact
	Requires	DEATH
	NextMove        Air_death_cycle
	Sets            NOCOLLISION

	Type		male
	  Anim          male\impact_belly 4 47
	TEnd

	Type		huge
	  Anim          huge\impact_belly 4 47
	TEnd

	Type		fem
	  Anim          fem\impact_belly 4 47
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 20 30
	TEnd
MEnd

Move Air_death_cycle
	Requires	DEATH
	Flags		Cycle
	NextMove	Revive_belly_Pre_rooted
	Sets            NOCOLLISION DIENOW TARGET
	Wind		DEATHWIND

	Type		male
	  Anim          male\impact_belly 48 50
	TEnd

	Type		huge
	  Anim          huge\impact_belly 48 50
	TEnd

	Type		fem
	  Anim          fem\impact_belly 48 50
	TEnd

	Type		Rularuu_male
	  Anim          male\mystic_float 1
	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
	TEnd

	Type		Wisp
	  Anim          wisp\freeze 20 30
	TEnd
MEnd

################################
### Special Camera Death (Air) ##
################################
#
#
#Move cam_Pre_Air_Death
#	Requires       FLY HIT DEATH Camera ##(replace test8 w\ CAMERA bit)
#	Interrupts	<DeathIRQ> <HitIRQ> Air_Death
#	NextMove        cam_Air_Death
#	Priority        4
#	SticksOnChild   DEATH
#
#	Type		male
#	  Anim          hoverbot\air_death 1 5
#	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
#	  PlayFx        Explosions\medium\Expolsion_med.fx
#	  PlayFx        WORLD\Ruins\MediumFireFast.fx 0 Constant
#	  PlayFx        Generic\dead_cambot.fx 0 Constant
#	TEnd
#MEnd
#
#Move cam_Air_Death
#	Requires	AIR DEATH Camera
#	Flags		ReqInputs
#	NextMove        cam_Air_death_fall
#	Priority        3
#	SticksOnChild   DEATH
#
#	Type		male
#	  Anim          hoverbot\air_death 6 25
#	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
#	  PlayFx        WORLD\Ruins\MediumFireFast.fx 0 Constant
#	  PlayFx        Generic\dead_cambot.fx 0 Constant
#	TEnd
#MEnd
#
#Move cam_Air_death_fall
#	Requires	AIR DEATH Camera
#	Flags		Cycle
#	NextMove        cam_Air_death_impact
#	Priority        3
#	SticksOnChild   DEATH
#
#	Type		male
#	  Anim          hoverbot\air_death 26 100
#	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
#	  PlayFx        WORLD\Ruins\MediumFireFast.fx 0 Constant
#	  PlayFx        Generic\dead_cambot.fx 0 Constant
#	TEnd
#MEnd
#
#Move cam_Air_death_impact
#	Requires	DEATH Camera
#	NextMove        cam_Air_death_cycle
#	Priority        2
#	Sets            NOCOLLISION
#	SticksOnChild   DEATH
#
#	Type		male
#	  Anim          hoverbot\air_death 26 50
#	  PlayFX	Explosions\TurretExplosion\LargeTurretExplosion.Fx
#	  PlayFx        Explosions\medium\Expolsion_med.fx
#	  PlayFx        WORLD\Ruins\MediumFireFast.fx 0 Constant
#	  PlayFx        Generic\dead_cambot.fx 0 Constant
#	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
#	TEnd
#MEnd
#
#Move cam_Air_death_cycle
#	Requires	DEATH
#	Flags		Cycle
#	Priority        2
#	Sets            NOCOLLISION DIENOW TARGET
#	SticksOnChild   DEATH
#
#	Type		male
#	  Anim          hoverbot\air_death 26 50
#	  PlayFx        GENERIC\ENTITYVANISH.FX 0 CONSTANT
#	  PlayFx        WORLD\Ruins\MediumFireFast.fx 0 Constant
#	  PlayFx        Generic\dead_cambot.fx 0 Constant
#	TEnd
#MEnd

#############################################################

Move Player_TPFriend #Special move for teleporting corpse
	Requires	PLAYER HIT TELEPORT NO TARGET
	Priority        105
	Interrupts	<RaiseDeadIRQ>
	Member		<RaiseDeadIRQ>
	Flags		NOTSELECTABLE ##CYCLE
	Sets            NOCOLLISION
	Wind		DEATHWIND
	NextMove	postdeath2

	Type		male
	  Anim          male\death_front 129 139
	TEnd

	Type		huge
	  Anim          huge\death_front 129 139
	TEnd

	Type		fem
	  Anim          fem\death_front 129 139
	TEnd
MEnd


#############################################################
## Hit Death Collapse (use on npc's that you want to fake die then drop into body_collapse encounter
#############################################################

Move HitDeath_Collapse
	Requires	DEATH DEATHCOLLAPSE
	Priority        104
	Interrupts	<DeathIRQ> <HitIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        postdeath_Collapse
	Flags		reqinputs cantmove

	Type		male
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		huge
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		fem
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		sprocket
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Carnival_masks
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		fem_floater
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		male_teledeath
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Rularuu_male
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Wisp
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		male_floater_Pirate
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Statesman_Escape
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Statesman
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		LordRecluse_Escape
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Romulus
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Pet_ElectricGremlin
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		InvisibleMaleTarget_Silent
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		P_AntiMatter
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		V_Shalice
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		P_Apparitions_Minion
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		P_Apparitions_Minion_Merged
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		P_Apparitions_Lieutenant
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd

	Type		Abomination
	  Anim          male\Recluse_Cutscene01 1 75
	TEnd


MEnd

Move postdeath_Collapse
	Requires	DEATHCOLLAPSE
	#Member		<DeathIRQ>
	Flags		Cycle REQINPUTS

	Type		male
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		huge
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		fem
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Carnival_masks
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		fem_floater
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		male_teledeath
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Rularuu_male
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Wisp
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		male_floater_Pirate
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Statesman_Escape
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Statesman
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		LordRecluse
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		LordRecluse_Escape
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Romulus
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Pet_ElectricGremlin
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		P_AntiMatter
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		V_Shalice
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

	Type		Abomination
	  Anim          male\Recluse_Cutscene02 1 105
	TEnd

MEnd

##########################################################