#############################################################
## warwalkers_seq.inc
#############################################################

###################
###### Default Death   ######
###################

Move Warwalker_Default_Death
	Requires	DEATH
	Priority	101
	Interrupts	<DeathIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        Warwalker_Default_postdeath
	Sets		DIENOW TARGET
	flags		nosizescale

	Type		Warworks_WarWalker
	  Anim          5cw\death_front 47 129
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Death.fx 1
	Scale		0.80
	TEnd

	Type		Warworks_Siege
	  Anim          5cw\death_front 47 129
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Death.fx 1
	  PlayFX	ENEMYFX\Praetorian\Clockwork\Death01.fx 0 CONSTANT
	Scale		0.80
	TEnd

	Type		Sentinel
	  Anim          5cw\death_front 47 129
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_Death.fx
	Scale		0.80
	TEnd

MEnd

Move Warwalker_Default_postdeath
	Requires	DEATH
	Flags		Cycle
	Sets            NOCOLLISION DIENOW TARGET
	NextMove	Revive_belly_Pre_rooted

	Type		Warworks_WarWalker
	  Anim          5cw\death_front 129 130
	TEnd

	Type		Warworks_Siege
	  Anim          5cw\death_twitch 1
	  PlayFX	ENEMYFX\Praetorian\Clockwork\Death01.fx 0 CONSTANT
	TEnd

	Type		Sentinel
	  Anim          5cw\death_front 129 130
	TEnd

MEnd

###################
###### Hit Death   ######
###################

Move Warwalker_HitDeath
	Requires	HIT DEATH
	Priority        102
	Interrupts	<DeathIRQ> <ReactIRQ>
	Member		<RaiseDeadIRQ>
	NextMove        Warwalker_Default_postdeath
	Sets            DIENOW TARGET

	Type		Warworks_WarWalker
	  Anim          5cw\death_front 47 129
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Death.fx 1
	  Scale		0.80
	TEnd

	Type		Warworks_Siege
	  Anim          5cw\death_front 47 129
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Death.fx 1
	  PlayFX	ENEMYFX\Praetorian\Clockwork\Death01.fx 0 CONSTANT
	  Scale		0.80
	TEnd

	Type		Sentinel
	  Anim          5cw\death_front 47 129
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_Death.fx
	  Scale		0.80
	TEnd
MEnd

#############################################################
## Warworks POWERS
#############################################################

########################
###### Warworks_ParticleBurst     ######
########################

Move Warworks_ParticleBurst
	Requires	MACHINEGUN BLAST ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Warworks_ParticleBurst2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Warworks_WarWalker
	  Anim          5cw\particle_burst 1 70
	  PitchStart	12
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 20

	TEnd

	Type		Sentinel
	  Anim          5cw\particle_burst 1 70
	  PitchStart	12
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 20

	TEnd
MEnd

Move Warworks_ParticleBurst2
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Interpolate     0
	Flags		NoSizeScale PitchToTarget

	Type		Warworks_WarWalker
	  Anim          5cw\particle_burst 119
	 PitchEnd	129
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 22
	TEnd

	Type		Sentinel
	  Anim          5cw\particle_burst 119
	 PitchEnd	129
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 22
	TEnd
MEnd

################################
###### Warworks_ParticleHeavy ######
################################

Move Warworks_ParticleHeavy
	Requires	COMBAT MEGABLAST ENDGAME
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Warworks_ParticleHeavypost
	Sets            CANTMOVE

	Type		Warworks_WarWalker
	  Anim          5cw\particle_heavy_burst 1 110
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 60
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 75
	TEnd

	Type		Sentinel
	  Anim          5cw\particle_heavy_burst 1 110
#	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 60
   	  PlayFX	Events\EndGame\Underground\Hamidon_Foot_Left.fx 75
	TEnd
MEnd

Move Warworks_ParticleHeavypost
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_15
	Scale           0.50

	Type		Warworks_WarWalker
	  Anim          5cw\particle_heavy_burst 110
	TEnd

	Type		Sentinel
	  Anim          5cw\particle_heavy_burst 110
	TEnd
MEnd

###################
###### QUICK SniperBlast (hands)  ######
###################

Move Warworks_ChargedShot
	Requires	COMBAT HIGH SNIPERBLAST ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Warworks_ChargedShot2
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		Warworks_WarWalker
	  Anim          5cw\charged_shot 1 100
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 14
	PitchAngle	0
	  PitchEnd	81
	TEnd

	Type		Sentinel
	  Anim          5cw\charged_shot 1 100
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 14
	PitchAngle	0
	  PitchEnd	81
	TEnd
MEnd

Move Warworks_ChargedShot2
	Requires	COMBAT
	Priority        1
	Member		<ReactIRQ> <DeathIRQ> <PostMoveCycle> <AttackIRQ> <BlockIRQ>
	NextMove        CombatReady_15

	Type		Warworks_WarWalker
	  Anim          5cw\charged_shot 100
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 10
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 30
	TEnd

	Type		Sentinel
	  Anim          5cw\charged_shot 100
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 10
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 30
	TEnd
MEnd

###################
###### War_Walker_Smash ######
###################

Move War_Walker_Smash_R
	Requires	COMBAT PUNCH ENDGAME
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	Flags		Cantmove
	NextMove        War_Walker_Smash_R_post
	SticksOnChild	PUNCH RIGHT

	Type		Warworks_WarWalker
	  Anim          5cw\smash_right 1 110
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 20
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Smash.fx 45
	TEnd

	Type		Sentinel
	  Anim          5cw\smash_right 1 110
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 20
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Smash.fx 45
	TEnd
MEnd

Move War_Walker_Smash_R_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Warworks_WarWalker
	  Anim          5cw\smash_right 111
	TEnd

	Type		Sentinel
	  Anim          5cw\smash_right 111
	TEnd
MEnd

##############################################################

Move War_Walker_Smash_L
	SticksOnChild	PUNCH LEFT
	Requires	COMBAT PUNCH ENDGAME
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	Flags		Cantmove
	NextMove        War_Walker_Smash_L_post

	Type		Warworks_WarWalker
	  Anim          5cw\smash_left 1 110
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 20
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 40
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Smash.fx 45
	TEnd

	Type		Sentinel
	  Anim          5cw\smash_left 1 110
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 20
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_FootSoft.fx 40
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Smash.fx 45
	TEnd
MEnd

Move War_Walker_Smash_L_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Warworks_WarWalker
	  Anim          5cw\smash_left 111
	TEnd

	Type		Sentinel
	  Anim          5cw\smash_left 111
	TEnd
MEnd


#############################################################

Move War_Walker_Smashcycle_15
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        War_Walker_Smashcycle
	Priority        1
	Scale           0.50

	Type		Warworks_WarWalker
	  Anim          5cw\claw_cycle 15 31
	TEnd

	Type		Sentinel
	  Anim          5cw\claw_cycle 15 31
	TEnd
MEnd

#############################################################

Move War_Walker_Smashcycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle
	NextMove        postReady_back
	Priority        1
	Scale           0.50

	Type		Warworks_WarWalker
	  Anim          5cw\claw_cycle 1
	TEnd

	Type		Sentinel
	  Anim          5cw\claw_cycle 1
	TEnd
MEnd

######################
###### pummel  ######
#####################

Move Pummel_pre
	Requires	COMBAT HAND SWIPE ENDGAME
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Pummel
	Sets            CANTMOVE

	Type		Warworks_WarWalker
	  Anim          5cw\Pummel 1 110
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 50
	TEnd

	Type		Sentinel
	  Anim          5cw\Pummel 1 110
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 50
	TEnd
MEnd

#############################################################

Move Pummel
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Warworks_WarWalker
	  Anim          5cw\Pummel 110
	TEnd

	Type		Sentinel
	  Anim          5cw\Pummel 110
	TEnd
MEnd


###################
###### Orbital Lance ######
###################

Move Warwalker_orbital_lance_lock
	Requires	COMBAT BOOSTUP LOCKDOWN ENDGAME HIGH
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Warwalker_orbital_lance_tell
#	Sets            CANTMOVE
	Flags		nosizescale

	Type		Warworks_WarWalker
	  Anim          5cw\orbital_lance 1 80
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 33
	TEnd

	Type		Sentinel
	  Anim          5cw\orbital_lance 1 80
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 33
	TEnd
MEnd


Move Warwalker_orbital_lance_tell
	Requires	COMBAT BOOSTUP ENDGAME LOW
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Warwalker_orbital_lance_attack
	Flags		nosizescale

	Type		Warworks_WarWalker
	  Anim          5cw\orbital_lance 80 140
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 55
	TEnd

	Type		Sentinel
	  Anim          5cw\orbital_lance 80 140
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoLo.fx 1
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 55
	TEnd
MEnd

Move Warwalker_orbital_lance_attack
	Requires	COMBAT BLAST ENDGAME
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Warwalker_orbital_lance_post
	Flags		nosizescale

	Type		Warworks_WarWalker
	  Anim          5cw\orbital_lance 140 200
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoAttack.fx 30
	TEnd

	Type		Sentinel
	  Anim          5cw\orbital_lance 140 200
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoAttack.fx 30
	TEnd
MEnd


Move Warwalker_orbital_lance_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		nosizescale

	Type		Warworks_WarWalker
	  Anim          5cw\orbital_lance 200
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 33
	TEnd

	Type		Sentinel
	  Anim          5cw\orbital_lance 200
  	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_ServoHi.fx 1
   	  PlayFX	ENEMYFX\Praetorian\Warworks\Warwalker_Foot.fx 33
	TEnd
MEnd

##########################################################
##################
###### Boost_up react  ######
###################

Move Hit_Boostup_Warwalker
	Requires	BOOSTAURA NOTROOTED QUICK
	Priority        3
	Interrupts	<ReactIRQ>
	Member		<ReactIRQ> <DeathIRQ> <HitIRQ> <AttackIRQ>
	NextMove        Ready


	Type		Warworks_WarWalker
	  Anim          5cw\drain 1
	TEnd

	Type		Sentinel
	  Anim          5cw\drain 1
	TEnd
MEnd

###################
###### Ground Slam  ######
###################

Move Warwalker_Ground_Slam
	Requires	COMBAT SMASH ENDGAME
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	NextMove        Warwalker_Ground_Slam_post
	Sets            CANTMOVE

	Type		Warworks_WarWalker
	  Anim          5cw\Ground_Slam 1 145
   	  PlayFX		ENEMYFX\Praetorian\Warworks\Warwalker_Smash.fx 80
	TEnd

	Type		Sentinel
	  Anim          5cw\Ground_Slam 1 145
	  PlayFX	ENEMYFX\BanishedPantheon\BanishedPantheon_Revamped\Sentinel_FrigidSmash_VOX.fx
	TEnd
MEnd

Move Warwalker_Ground_Slam_post
	Requires	COMBAT
	Priority        1
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady

	Type		Warworks_WarWalker
	  Anim          5cw\Ground_Slam 146 160
	TEnd

	Type		Sentinel
	  Anim          5cw\Ground_Slam 146 160
	TEnd
MEnd

###################
###### Powered Down - Pose Only for Environment  ######
###################

Move WarWalker_powereddown_pose
	Requires	ENCOUNTER ENDGAME
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		Cycle NoSizeScale

	Type		Warworks_WarWalker
	  Anim          5cw\goliath_warwalker_powereddown 1 1
	TEnd

	Type		Sentinel
	  Anim          5cw\goliath_warwalker_powereddown 1 1
	TEnd


MEnd

#########################
###War Walker Air Drop######
#########################

Move WarWalker_Air_Drop_Pre
	Requires	SPAWN ENDGAME ALTERNATE
	Priority 	3
	Interrupts	<AttackIRQ> <Ready> <Combat> <WepCombat>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <basicVANDAL>
	Flags		NoSizeScale CANTMOVE NOCOLLISION
	NextMove	WarWalker_Air_Drop

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_Air_Drop 1 25
	PlayFX		GENERIC\entityFadeIn.fx 0 constant
	 PlayFX		ENEMYFX\Praetorian\Warworks\Fly_Thrusters.fx 0 CONSTANT
	TEnd

	Type		Sentinel
	  Anim          5cw\Goliath_WarWalker_Air_Drop 1 25
	PlayFX		GENERIC\entityFadeIn.fx 0 constant
	 PlayFX		ENEMYFX\Praetorian\Warworks\Fly_Thrusters.fx 0 CONSTANT
	TEnd

MEnd

Move WarWalker_Air_Drop
	Interrupts	<AttackIRQ> <Ready> <Combat> <WepCombat>
	Member		<ReactIRQ> <DeathIRQ> <AttackIRQ> <basicVANDAL>
	Flags		NotSelectable NoSizeScale
	Sets            CANTMOVE
	NextMove	WarWalker_Air_Drop_Post

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_Air_Drop 26
	 PlayFX		ENEMYFX\Praetorian\Warworks\Fly_Thrusters.fx 0 CONSTANT
	TEnd

	Type		Sentinel
	  Anim          5cw\Goliath_WarWalker_Air_Drop 26
	 PlayFX		ENEMYFX\Praetorian\Warworks\Fly_Thrusters.fx 0 CONSTANT
	TEnd

MEnd

Move WarWalker_Air_Drop_Post
	Interrupts	<AttackIRQ> <Ready> <Combat> <WepCombat>
	Member		<Ready> <ReactIRQ> <DeathIRQ> <AttackIRQ> <basicVANDAL>
	Flags		NotSelectable NoSizeScale
	Sets            CANTMOVE
	Scale		0.50
	NextMove	Ready

	Type		Warworks_WarWalker
	  Anim          5cw\Goliath_WarWalker_jumppost 1 29
	  PlayFX	ENEMYFX\Praetorian\Warworks\Landing_Dust.fx   0
	  PlayFX	ENEMYFX\Praetorian\Warworks\Goliath_Foot_Left.fx 1
	  PlayFX	ENEMYFX\Praetorian\Warworks\Goliath_Foot_Right.fx 4
	TEnd

	Type		Sentinel
	  Anim          5cw\Goliath_WarWalker_jumppost 1 29
	  PlayFX	ENEMYFX\Praetorian\Warworks\Landing_Dust.fx   0
	  PlayFX	ENEMYFX\Praetorian\Warworks\Goliath_Foot_Left.fx 1
	  PlayFX	ENEMYFX\Praetorian\Warworks\Goliath_Foot_Right.fx 4
	TEnd
MEnd

#########################