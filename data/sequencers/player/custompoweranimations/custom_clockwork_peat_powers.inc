TypeDef male
	BaseSkeleton	male\skel_ready2
	ParentType	None
TypeDefEnd

TypeDef PraetorianClockworkMale
	BaseSkeleton	male\skel_ready2
	ParentType	male
TypeDefEnd

#############################################################
## COMBAT READIES
#############################################################

Move Wrist_Ready_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	NextMove	WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_wrist_combat_cycle 1
	TEnd

MEnd

Move WristCombatToReady
	Include sequencers\Members\Readies.inc
	NextMove	Ready

	Type		male
	  Anim          male\clockwork_wrist_combat_to_player_ready 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Resume.fx"
	TEnd
MEnd

#############################################################

Move Chest_Ready_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	NextMove	ChestCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chest_combat_cycle 1
	TEnd
MEnd

Move ChestCombatToReady
	Include sequencers\Members\Readies.inc
	NextMove	Ready

	Type		male
	  Anim          male\clockwork_chest_combat_to_player_ready 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Resume.fx"
	TEnd
MEnd

#############################################################

Move Shoulder_Ready_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	ShoulderCombatToReady
	Flags		Cycle PitchToTarget
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_shoulder_combat_cycle 1
	TEnd
MEnd

Move ShoulderCombatToReady
	Include sequencers\Members\Readies.inc
	NextMove	Ready

	Type		male
	  Anim          male\clockwork_shoulder_combat_to_ready 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Resume.fx"
	TEnd
MEnd

##################################
## Clockwork_Wrist - attack
##################################

Move Clockwork_plasmablast_EnterCombatStance
	Requires	COMBAT ROBOTBLAST ARMGUNBURST WEPSPAWN
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_plasmablast

	Type		male
	  Anim          male\clockwork_player_ready_to_wrist_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_plasmablast
	Requires	COMBAT ROBOTBLAST ARMGUNBURST
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_plasmablast_Post

	Type		male
	  Anim          male\clockwork_plasmablast 1 40
	TEnd
MEnd

Move Clockwork_plasmablast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Wrist_Ready_cycle
	NextMove	WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_plasmablast 40
	TEnd
MEnd

#############################################################

Move Clockwork_plasmaray_EnterCombatStance
	Requires	COMBAT ROBOTBLAST ARMGUNHEAVYBURST WEPSPAWN
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_plasmablast

	Type		male
	  Anim          male\clockwork_player_ready_to_wrist_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_plasmaray
	Requires	COMBAT ROBOTBLAST ARMGUNHEAVYBURST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_plasmaray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_plasmaray 1 70
	TEnd
MEnd

Move Clockwork_plasmaray_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Wrist_Ready_cycle
	NextMove	WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_plasmaray 70
	TEnd
MEnd

#############################################################

Move Clockwork_plasmaspray_EnterCombatStance
	Requires	COMBAT ROBOTBLAST ARMGUNCONE WEPSPAWN
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_plasmaspray

	Type		male
	  Anim          male\clockwork_player_ready_to_wrist_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_plasmaspray
	Requires	COMBAT ROBOTBLAST ARMGUNCONE
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_plasmaspray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Clockwork_plasmaspray 1 115
	TEnd
MEnd

Move Clockwork_plasmaspray_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Wrist_Ready_cycle
	NextMove	WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_plasmaspray 115
	TEnd
MEnd

##################################
## Clockwork_Chest - attack
##################################

Move Clockwork_chestcannon_EnterCombatStance
	Requires	COMBAT ROBOTBLAST CHESTCANNON BLAST WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_chestcannon

	Type		male
	  Anim          male\clockwork_player_ready_to_chest_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_chestcannon
	Requires	COMBAT ROBOTBLAST CHESTCANNON BLAST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_chestcannon_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_chestcannon 1 125
	TEnd
MEnd

Move Clockwork_chestcannon_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Chest_Ready_cycle
	NextMove	ChestCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chestcannon 125
	TEnd
MEnd

#############################################################

Move Clockwork_chestspray_EnterCombatStance
	Requires	COMBAT ROBOTBLAST CHESTCANNON CONE WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_chestspray

	Type		male
	  Anim          male\clockwork_player_ready_to_chest_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_chestspray
	Requires	COMBAT ROBOTBLAST CHESTCANNON CONE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_chestspray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_chestspray 1 110
	TEnd
MEnd

Move Clockwork_chestspray_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Chest_Ready_cycle
	NextMove	ChestCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chestspray 110
	TEnd
MEnd

##################################
## Clockwork_Shoulder - attack
##################################

Move Clockwork_shoulderblast_EnterCombatStance
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST BLAST WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_shoulderblast

	Type		male
	  Anim          male\clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_shoulderblast
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST BLAST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_shoulderblast_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_shoulderblast 1 28
	TEnd
MEnd

Move Clockwork_shoulderblast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Shoulder_Ready_cycle
	NextMove	ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_shoulderblast 28
	TEnd
MEnd


#############################################################

Move Clockwork_shoulderbarrage_EnterCombatStance
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST ARMGUNBARRAGE WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_shoulderbarrage

	Type		male
	  Anim          male\clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_shoulderbarrage
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST ARMGUNBARRAGE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_shoulderbarrage_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_shoulderbarrage 1 50
	TEnd
MEnd

Move Clockwork_shoulderbarrage_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Shoulder_Ready_cycle
	NextMove	ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_shoulderbarrage 57
	TEnd
MEnd

#############################################################

Move Clockwork_shoulderburst_EnterCombatStance
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST BURST WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_shoulderburst

	Type		male
	  Anim          male\clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_shoulderburst
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST BURST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_shoulderburst_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Clockwork_shoulderburst 1 105
	TEnd
MEnd

Move Clockwork_shoulderburst_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Shoulder_Ready_cycle
	NextMove	ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Clockwork_shoulderburst 105
	TEnd
MEnd

#############################################################

Move Clockwork_shoulderspray_EnterCombatStance
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST CONE WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_shoulderspray

	Type		male
	  Anim          male\clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Clockwork_shoulderspray
	Requires	COMBAT ROBOTBLAST SHOULDERBLAST CONE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_shoulderspray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_shoulderspray 1 110
	TEnd
MEnd

Move Clockwork_shoulderspray_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle PitchToTarget
	CycleMove	Shoulder_Ready_cycle
	NextMove	ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_shoulderspray 110
	TEnd
MEnd



######################################################
			### Air ###
######################################################

#############################################################
## COMBAT READIES
#############################################################

Move air_Wrist_Ready_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget
	NextMove	air_WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\air_clockwork_wrist_combat_cycle 1
	TEnd

MEnd

Move air_WristCombatToReady
	Include sequencers\Members\Readies_air.inc
	NextMove	air_Ready

	Type		male
	  Anim          male\air_clockwork_wrist_combat_to_player_ready 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Resume.fx"
	TEnd
MEnd

#############################################################

Move air_Chest_Ready_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget
	NextMove	air_ChestCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\air_clockwork_chest_combat_cycle 1
	TEnd
MEnd

Move air_ChestCombatToReady
	Include sequencers\Members\Readies_Air.inc
	NextMove	air_Ready

	Type		male
	  Anim          male\air_clockwork_chest_combat_to_player_ready 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Resume.fx"
	TEnd
MEnd

#############################################################

Move air_Shoulder_Ready_cycle
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	NextMove	air_ShoulderCombatToReady
	Flags		Cycle PitchToTarget
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\air_clockwork_shoulder_combat_cycle 1
	TEnd
MEnd

Move air_ShoulderCombatToReady
	Include sequencers\Members\Readies_Air.inc
	NextMove	air_Ready

	Type		male
	  Anim          male\air_clockwork_shoulder_combat_to_player_ready 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Resume.fx"
	TEnd
MEnd

#############################################################
			## Reactions
#############################################################

#############################################################
### Combat Hit Quick
##############################################################
#
#Move Clockwork_HitQuick
#	Requires	COMBAT QUICK HIT
#	Priority        3
#	Interrupts	<HitIRQ> FearStun_cycle FearStun
#	Include sequencers\Members\Reactions.inc
#	NextMove        Wrist_Ready_cycle
#	Flags		HitReact
#
#	Type		PraetorianClockworkMale
#	  Anim          male\clockwork_HQ_mid_wrist 1
#	TEnd
#MEnd
#
##############################################################
#
#Move Clockwork_HitQuick_mid
#	Requires	COMBAT QUICK HIT
#	Priority        3
#	Interrupts	<HitIRQ>
#	Include sequencers\Members\Reactions.inc
#	NextMove        Wrist_Ready_cycle
#	Flags		HitReact
#
#	Type		PraetorianClockworkMale
#	  Anim          male\clockwork_HQ_mid_wrist 1
#	TEnd
#
#MEnd

#########################################
		## Air Combat ##
########################################

##################################
## Air_Clockwork_Wrist - attack
##################################

Move Air_Clockwork_plasmablast_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST ARMGUNBURST WEPSPAWN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_plasmablast

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_wrist_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_plasmablast
	Requires	FLY COMBAT ROBOTBLAST ARMGUNBURST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_plasmablast_Post

	Type		male
	  Anim          male\Air_clockwork_plasmablast 1 40
	TEnd
MEnd

Move Air_Clockwork_plasmablast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Wrist_Ready_cycle
	NextMove	Air_WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_plasmablast 40
	TEnd
MEnd

#############################################################

Move Air_Clockwork_plasmaray_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST ARMGUNHEAVYBURST WEPSPAWN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_plasmablast

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_wrist_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_plasmaray
	Requires	FLY COMBAT ROBOTBLAST ARMGUNHEAVYBURST
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_plasmaray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_clockwork_plasmaray 1 70
	TEnd
MEnd

Move Air_Clockwork_plasmaray_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Wrist_Ready_cycle
	NextMove	Air_WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_plasmaray 70
	TEnd
MEnd

#############################################################

Move Air_Clockwork_plasmaspray_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST ARMGUNCONE WEPSPAWN
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_plasmaspray

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_wrist_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_plasmaspray
	Requires	FLY COMBAT ROBOTBLAST ARMGUNCONE
	Priority	14
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_plasmaspray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_Clockwork_plasmaspray 1 115
	TEnd
MEnd

Move Air_Clockwork_plasmaspray_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Wrist_Ready_cycle
	NextMove	Air_WristCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_plasmaspray 115
	TEnd
MEnd

##################################
## Air_Clockwork_Chest - attack
##################################

Move Air_Clockwork_chestcannon_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST CHESTCANNON BLAST WEPSPAWN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_chestcannon

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_chest_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_chestcannon
	Requires	FLY COMBAT ROBOTBLAST CHESTCANNON BLAST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_chestcannon_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_clockwork_chestcannon 1 125
	TEnd
MEnd

Move Air_Clockwork_chestcannon_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Chest_Ready_cycle
	NextMove	Air_ChestCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_chestcannon 125
	TEnd
MEnd

#############################################################

Move Air_Clockwork_chestspray_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST CHESTCANNON CONE WEPSPAWN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <attackirq>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_chestspray

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_chest_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_chestspray
	Requires	FLY COMBAT ROBOTBLAST CHESTCANNON CONE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_chestspray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_clockwork_chestspray 1 110
	TEnd
MEnd

Move Air_Clockwork_chestspray_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Chest_Ready_cycle
	NextMove	Air_ChestCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_chestspray 110
	TEnd
MEnd

##################################
## Air_Clockwork_Shoulder - attack
##################################

Move Air_Clockwork_shoulderblast_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST BLAST WEPSPAWN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_shoulderblast

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_shoulderblast
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST BLAST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_shoulderblast_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_clockwork_shoulderblast 1 28
	TEnd
MEnd

Move Air_Clockwork_shoulderblast_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Shoulder_Ready_cycle
	NextMove	Air_ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_shoulderblast 28
	TEnd
MEnd


#############################################################

Move Air_Clockwork_shoulderbarrage_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST ARMGUNBARRAGE WEPSPAWN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <attackirq>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_shoulderbarrage

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_shoulderbarrage
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST ARMGUNBARRAGE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_shoulderbarrage_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_clockwork_shoulderbarrage 1 50
	TEnd
MEnd

Move Air_Clockwork_shoulderbarrage_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Shoulder_Ready_cycle
	NextMove	Air_ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_shoulderbarrage 57
	TEnd
MEnd

#############################################################

Move Air_Clockwork_shoulderburst_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST BURST WEPSPAWN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_shoulderburst

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_shoulderburst
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST BURST
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_shoulderburst_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_Clockwork_shoulderburst 1 105
	TEnd
MEnd

Move Air_Clockwork_shoulderburst_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Shoulder_Ready_cycle
	NextMove	Air_ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_Clockwork_shoulderburst 105
	TEnd
MEnd

#############################################################

Move Air_Clockwork_shoulderspray_EnterCombatStance
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST CONE WEPSPAWN
	Priority	16
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Air_Clockwork_shoulderspray

	Type		male
	  Anim          male\Air_clockwork_player_ready_to_shoulder_combat 1
	  PlayFX	"ENEMYFX\Clockwork\antimatter\Clockwork_ShoulderAttack_Enter.fx"
	TEnd
MEnd

Move Air_Clockwork_shoulderspray
	Requires	FLY COMBAT ROBOTBLAST SHOULDERBLAST CONE
	Priority	15
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Air_Clockwork_shoulderspray_Post
	Flags		PitchToTarget

	Type		male
	  Anim          male\Air_clockwork_shoulderspray 1 110
	TEnd
MEnd

Move Air_Clockwork_shoulderspray_Post
	Requires	FLY COMBAT
	Include sequencers\Members\Combat_Air.inc
	Flags		Cycle PitchToTarget reqinputs
	CycleMove	Air_Shoulder_Ready_cycle
	NextMove	Air_ShoulderCombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\Air_clockwork_shoulderspray 110
	TEnd
MEnd

#############################################################
		## Air Reactions ##
#############################################################

####################
####### Air Combat Hit Quick  ######
####################
#
#Move A_Clockwork_HitQuick
#	Requires	FLY COMBAT QUICK HIT
#	Priority        4
#	Interrupts	<HitIRQ>
#	Include sequencers\Members\Reactions_Air.inc
#	NextMove        Air_Wrist_Ready_cycle
#	Flags		HitReact
#
#
#	Type		PraetorianClockworkMale
#	  Anim          male\air_clockwork_HQ_mid_wrist 1
#	TEnd
#
#MEnd
#
##############################################################
#
#Move A_Clockwork_HitQuick_L
#	Requires	FLY COMBAT QUICK HIT
#	Priority        4
#	Interrupts	<HitIRQ>
#	Include sequencers\Members\Reactions_Air.inc
#	NextMove        Air_Wrist_Ready_cycle
#	Flags		HitReact
#
#	Type		PraetorianClockworkMale
#	  Anim          male\air_clockwork_HQ_mid_wrist 1
#	TEnd
#
#MEnd

#############################################################