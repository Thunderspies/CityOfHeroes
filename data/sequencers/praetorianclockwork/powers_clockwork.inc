#########################################################
## Clockwork_Powers.INC - clockwork Power related sequencers
#########################################################

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
	NextMove	Ready_Break2

	Type		male
	  Anim          male\clockwork_wrist_combat_to_ready 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Resume.fx
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
	NextMove	Ready_Break2

	Type		male
	  Anim          male\clockwork_chest_combat_to_ready 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Resume.fx
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
	NextMove	Ready_Break2

	Type		male
	  Anim          male\clockwork_shoulder_combat_to_ready 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Resume.fx
	TEnd
MEnd

#############################################################

Move ChainGun_Ready_cycle
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove	ChainGun_CombatToReady
	Flags		Cycle #PitchToTarget
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chaingun_combat_cycle 1
	TEnd
MEnd

Move ChainGun_CombatToReady
	Include sequencers\Members\Readies.inc
	NextMove	Ready_Break2

	Type		male
	  Anim          male\clockwork_chaingun_combat_to_ready 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Resume.fx
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
	  Anim          male\clockwork_ready_to_wrist_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx
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
	  Anim          male\clockwork_ready_to_wrist_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx
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
	  Anim          male\clockwork_ready_to_wrist_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_WristAttack_Enter.fx
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

#############################################################

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
	  Anim          male\clockwork_ready_to_chest_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx
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
	  Anim          male\clockwork_ready_to_chest_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx
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

#############################################################

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
	  Anim          male\clockwork_ready_to_shoulder_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx
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
	  Anim          male\clockwork_ready_to_shoulder_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx
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
	  Anim          male\clockwork_ready_to_shoulder_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx
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
	  Anim          male\clockwork_ready_to_shoulder_combat 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_ChestAttack_Enter.fx
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

##################################
## Clockwork_ChainGun - attack
##################################

Move Clockwork_ChainGun_Shotgun_EnterCombatStance
	Requires	COMBAT ROBOTBLAST CHAINGUN SHOTGUN WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	#Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_ChainGun_Single_shot

	Type		male
	  Anim          male\clockwork_ready_to_ChainGun_combat 1 36
	TEnd
MEnd

Move Clockwork_ChainGun_Single_shot
	Requires	COMBAT ROBOTBLAST CHAINGUN SHOTGUN
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_ChainGun_Single_shot_Post
#	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_chaingun_single_shot 1 42
	TEnd
MEnd

Move Clockwork_ChainGun_Single_shot_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle #PitchToTarget
	CycleMove	ChainGun_Ready_cycle
	NextMove	ChainGun_CombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chaingun_single_shot 43
	TEnd
MEnd

#############################################################

Move Clockwork_ChainGun_Burst_EnterCombatStance
	Requires	COMBAT ROBOTBLAST CHAINGUN BURST WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	#Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_ChainGun_Burst

	Type		male
	  Anim          male\clockwork_ready_to_ChainGun_combat 1 36
	TEnd
MEnd

Move Clockwork_ChainGun_Burst
	Requires	COMBAT ROBOTBLAST CHAINGUN BURST
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_ChainGun_Burst_Post
#	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_chaingun_Burst 1 40
	TEnd
MEnd

Move Clockwork_ChainGun_Burst_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle #PitchToTarget
	CycleMove	ChainGun_Ready_cycle
	NextMove	ChainGun_CombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chaingun_Burst 41
	TEnd
MEnd

#############################################################

Move Clockwork_ChainGun_Grenade_EnterCombatStance
	Requires	COMBAT ROBOTBLAST CHAINGUN GRENADE WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	#Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_ChainGun_Grenade

	Type		male
	  Anim          male\clockwork_ready_to_ChainGun_combat 1 36
	TEnd
MEnd

Move Clockwork_ChainGun_Grenade
	Requires	COMBAT ROBOTBLAST CHAINGUN GRENADE
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_ChainGun_Grenade_Post
#	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_chaingun_Grenade 1 50
	TEnd
MEnd

Move Clockwork_ChainGun_Grenade_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle #PitchToTarget
	CycleMove	ChainGun_Ready_cycle
	NextMove	ChainGun_CombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chaingun_Grenade 51
	TEnd
MEnd

#############################################################

Move Clockwork_ChainGun_Fullauto_EnterCombatStance
	Requires	COMBAT ROBOTBLAST CHAINGUN FULLAUTO WEPSPAWN
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	#Flags		PitchToTarget
	Sets		CANTMOVE
	NextMove	Clockwork_ChainGun_Full_Auto

	Type		male
	  Anim          male\clockwork_ready_to_ChainGun_combat 1 36
	TEnd
MEnd

Move Clockwork_ChainGun_Full_Auto
	Requires	COMBAT ROBOTBLAST CHAINGUN FULLAUTO
	Priority	4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_ChainGun_Full_Auto_Post
#	Flags		PitchToTarget

	Type		male
	  Anim          male\clockwork_chaingun_full_auto 1 112
	TEnd
MEnd

Move Clockwork_ChainGun_Full_Auto_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		Cycle #PitchToTarget
	CycleMove	ChainGun_Ready_cycle
	NextMove	ChainGun_CombatToReady
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\clockwork_chaingun_full_auto 113
	TEnd
MEnd

#############################################################
Move SmashCast
	Requires	COMBAT SMASH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        SmashCastpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\2Hsmash 1 45
	TEnd
MEnd

Move SmashCastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_Half
	Scale           0.80
	Flags		NoSizeScale ReqInputs
	SticksOnChild	COMBAT

	Type		male
	  Anim          male\2Hsmash 45
	TEnd
MEnd

## 1 HAND SMASH ###########################################################

Move OneHandSmash_Right
	Requires	COMBAT PUNCH SMASH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        OneHandSmash_Right2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\1H_smash 12 36
	TEnd
MEnd

Move OneHandSmash_Right2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        OneHandSmash_Right_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 36 59
	TEnd
MEnd

Move OneHandSmash_Right_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_Half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 59 82
	TEnd
MEnd

#############################################################

Move OneHandSmash_Left
	Requires	COMBAT PUNCH SMASH
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        OneHandSmash_Left2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\1H_smash 96 120
	TEnd
MEnd

Move OneHandSmash_Left2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        OneHandSmash_Left_Post
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 120 140
	TEnd
MEnd

Move OneHandSmash_Left_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_Half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\1H_smash 140
	TEnd
MEnd

## STOPCAST ###########################################################

Move Stopcast
	Requires	COMBAT STOPCAST
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Stopcast_Post
	Flags		PitchToTarget NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\stop 1 60
	  PitchAngle	0.5
	TEnd
MEnd

Move Stopcast_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady_Half
	Scale           0.50
	Flags		PitchToTarget ReqInputs NoSizeScale

	Type		male
	  Anim          male\stop 60
	  PitchEnd	64
	  PitchAngle	0.5
	TEnd
MEnd

#############################################################

Move Heal
	Requires	COMBAT HEAL
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Heal_Post
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\heal 1 35
	  PitchStart	8
	  PitchAngle	0.1
	TEnd
MEnd

Move Heal_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\heal 35
	  PitchEnd	63
	  PitchAngle	0.1
	TEnd
MEnd

#############################################################

Move PlantDevice_Pre
	Requires	LOOT QUICK
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <MoveIRQ>
	NextMove        PlantDevice_Cycle
	Scale           0.70

	Type		male
	  Anim          male\Loot 3 19
	TEnd
MEnd

Move PlantDevice_Cycle
	Requires	LOOT QUICK
	Member		<DeathIRQ> <HitIRQ> <MoveIRQ>
	Flags		Cycle
	NextMove        PlantDevice_Post
	Scale		0.5

	Type		male
	  Anim          male\examine_hero 1
	TEnd
MEnd

Move PlantDevice_Post
	Include sequencers\Members\Readies.inc
	NextMove        Ready

	Type		male
	  Anim          male\Loot 26
	TEnd
MEnd

#############################################################

#############################################################
## ACU - WarWorks FULL AUTO
#############################################################

Move ACU_FullAuto_Draw
	Requires	COMBAT WEAPON GUN MACHINEGUN FULLAUTO SPAWNBACK ENDGAME
	Priority        7
	Interrupts	<AttackIRQ> <Defiance>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ACU_FullAuto
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move ACU_FullAuto
	Requires	COMBAT WEAPON GUN MACHINEGUN FULLAUTO ENDGAME
	Priority	6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Interpolate	5
	NextMove        ACU_FullAuto_Post
	Flags		PitchToTarget NOSIZESCALE FixArmReg
	Sets            CANTMOVE

	Type		male
	  Anim          male\machinegun_long 1 178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 31 151 #178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 31 151 #178
	  PitchAngle	0.3
	  PitchEnd	172
	TEnd
MEnd

Move ACU_FullAuto_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ACU_ShotgunReady
	Flags		FixArmReg NOSIZESCALE ReqInputs

	Type		male
	  Anim          male\machinegun_long 179
	TEnd

	Type		huge
	  Anim          huge\machinegun_long 179
	TEnd

	Type		fem
	  Anim          fem\machinegun_long 179
	TEnd
MEnd

####################################################
Move ACU_ShotgunReady
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle FixArmReg
	NextMove        ACU_PostShotgun
	Scale           0.50

	Type		male
	  Anim          male\rocket_cycle 1 31
	TEnd

	Type		huge
	  Anim          huge\rocket_cycle 1 31
	TEnd

	Type		fem
	  Anim          fem\rocket_cycle 1 31
	TEnd
MEnd


#############################################################

Move ACU_D_Shotgun_cycle
	Requires	COMBAT WEAPON GUN
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	Flags		Cycle FixArmReg
	NextMove        ACU_PostShotgun
	Scale           0.50

	Type		male
	  Anim          male\shotgun_cycle 1
	TEnd

	Type		huge
	  Anim          huge\shotgun_cycle 1
	TEnd

	Type		fem
	  Anim          fem\shotgun_cycle 1
	TEnd
MEnd
#############################################################

Move ACU_PostShotgun
	Include sequencers\Members\Readies.inc
	NextMove        Ready

	Type		male
	  Anim          male\post_shotgun 1
	TEnd

	Type		huge
	  Anim          huge\postback 3 40
	TEnd

	Type		fem
	  Anim          fem\post_ready 1
	TEnd
MEnd

#############################################################
## BUCKSHOT
#############################################################

Move ACU_Grenade_Draw
	Requires	COMBAT WEAPON GUN LOW SHOTGUN SPAWNBACK ENDGAME
	Priority        7
	Interrupts	<AttackIRQ> <Defiance>
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ACU_Grenade
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		huge
	  Anim          huge\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd

	Type		fem
	  Anim          fem\draw_shotgun_point 1 31
	  PitchAngle	0.3
	  PitchEnd	38
	TEnd
MEnd

Move ACU_Grenade
	Requires	COMBAT WEAPON GUN LOW SHOTGUN ENDGAME
	Priority        6
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        ACU_Grenade_post
	Sets            CANTMOVE
	Flags		PitchToTarget NOSIZESCALE FixArmReg

	Type		male
	  Anim          male\shotgun_shoot 25 51
	  PitchAngle	0.3
	TEnd

	Type		huge
	  Anim          huge\shotgun_shoot 25 51
	  PitchAngle	0.3
	TEnd

	Type		fem
	  Anim          fem\shotgun 25 51
	  PitchAngle	0.3
	TEnd
MEnd

Move ACU_Grenade_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_Weapon_TwoHand.inc
	NextMove        ACU_D_Shotgun_cycle
	Flags		PitchToTarget NOSIZESCALE FixArmReg ReqInputs
	Scale           0.50

	Type		male
	  Anim          male\shotgun_shoot 52
	TEnd

	Type		huge
	  Anim          huge\shotgun_shoot 52
	TEnd

	Type		fem
	  Anim          fem\shotgun 52
	TEnd
MEnd

################################################
###### MKVI Victoria Warworks Endgame ###########################################
########################################

## LIGHT OPENING ###########################################################

Move DualWield_LightOpeningA_Draw
	Requires	COMBAT WEAPON DUAL ATTACK QUICK SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_LightOpeningA
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		PraetorianClockwork_Victoria
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_LightOpeningA
	Requires	COMBAT WEAPON DUAL ATTACK QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_LightOpeningA_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_LightOpeningA 2 33
	TEnd
MEnd

Move DualWield_LightOpeningA_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_LightOpeningA 34
	TEnd
MEnd


## LIGHT OPENING B ###########################################################

Move DualWield_LightOpeningB_Draw
	Requires	COMBAT WEAPON DUAL ATTACK QUICK SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_LightOpeningB
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		PraetorianClockwork_Victoria
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_LightOpeningB
	Requires	COMBAT WEAPON DUAL ATTACK QUICK
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_LightOpeningB_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_LightOpeningB 2 33
	TEnd
MEnd

Move DualWield_LightOpeningB_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_LightOpeningB 34
	TEnd
MEnd


## MODERATE OPENING ###########################################################

Move DualWield_ModerateOpening_Draw
	Requires	COMBAT WEAPON DUAL ATTACK MID ONE SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_ModerateOpening
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		PraetorianClockwork_Victoria
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_ModerateOpening
	Requires	COMBAT WEAPON DUAL ATTACK MID ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_ModerateOpening_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_ModerateOpening 2 43
	TEnd
MEnd

Move DualWield_ModerateOpening_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_ModerateOpening 44
	TEnd
MEnd


## MODERATE BRIDGE ###########################################################

Move DualWield_ModerateBridge_Draw
	Requires	COMBAT WEAPON DUAL ATTACK MID TWO SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_ModerateBridge
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		PraetorianClockwork_Victoria
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_ModerateBridge
	Requires	COMBAT WEAPON DUAL ATTACK MID TWO
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_ModerateBridge_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_ModerateBridge 2 33
	TEnd
MEnd

Move DualWield_ModerateBridge_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_ModerateBridge 34
	TEnd
MEnd


## AOE BRIDGE ###########################################################

Move DualWield_AOEBridge_Draw
	Requires	COMBAT WEAPON DUAL ATTACK SPIN SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_AOEBridge
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		PraetorianClockwork_Victoria
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_AOEBridge
	Requires	COMBAT WEAPON DUAL ATTACK SPIN
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_AOEBridge_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_AOEBridge 2 70
	TEnd
MEnd

Move DualWield_AOEBridge_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_AOEBridge 71
	TEnd
MEnd

###################
###### mouth_eye blast  ######
###################

Move Mouth
	Requires	COMBAT HEADCAST
	Priority	2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        PostMouth
	Sets            CANTMOVE
	Flags		PitchToTarget

	Type		PraetorianClockwork_Victoria
	  Anim          male\eye 1 50
	  PitchStart	1
	  PitchAngle	0.1
	TEnd
MEnd

Move PostMouth
	Include sequencers\Members\Combat.inc
	NextMove        CombatBlend

	Type		PraetorianClockwork_Victoria
	  Anim          male\eye 50 60
	TEnd
MEnd

###################
###### Boost_down  ######
###################

Move Boost_D
	Requires	BOOSTDOWN
	Priority        1
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        Boost_Dpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		PraetorianClockwork_Victoria
	  Anim          fem\boostdown 1 10
	TEnd
MEnd

Move Boost_Dpost
	Member		<DeathIRQ> <AttackIRQ> <BlockIRQ> <MoveIRQ> <IRQEmotesCombat>
	NextMove        BoostDnReady
	Flags		NoSizeScale

	Type		PraetorianClockwork_Victoria
	  Anim          fem\boostdown 10
	TEnd
MEnd

Move BoostDnReady
	Include sequencers\Members\Readies.inc
	Flags		Cycle NoSizeScale

	Type		PraetorianClockwork_Victoria
	  Anim          fem\Skel_ready2 1 60
	TEnd
MEnd

## PLACATE_BROADSWORD ###########################################################

Move Broadsword_Placate_Draw
	Requires	COMBAT WEAPON MACE PLACATE SPAWNBACK
	Priority        55
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Broadsword_Placate
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		PraetorianClockwork_Victoria
	  Anim          male\draw_axe 1 30
	TEnd
MEnd

Move Broadsword_Placate
	Requires	COMBAT WEAPON MACE PLACATE
	Priority        4
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Broadsword_Placate_Post
	Sets		CANTMOVE
	Flags		NOSIZESCALE

	Type		PraetorianClockwork_Victoria
	  Anim          male\placate_broadsword 1 45
	TEnd
MEnd

Move Broadsword_Placate_Post
	Requires	COMBAT WEAPON
	Include sequencers\Members\Combat_Weapon.inc
	NextMove        WepReady
	Flags		NOSIZESCALE ReqInputs

	Type		PraetorianClockwork_Victoria
	  Anim          male\placate_broadsword 45
	TEnd
MEnd

## HIGH LOW ###########################################################

Move DualWield_HighLow_Draw
	Requires	COMBAT WEAPON DUAL ATTACK FIERCE SPAWNBACK
	Priority        56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	DualWield_HighLow
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		1.25

	Type		PraetorianClockwork_Victoria
	  Anim		male\DualBlade_Draw 5 31
	TEnd
MEnd

Move DualWield_HighLow
	Requires	COMBAT WEAPON DUAL ATTACK FIERCE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        DualWield_HighLow_Post
	Sets            CANTMOVE
	Flags		NOSIZESCALE
	Scale		0.75

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_HighLow 2 75
	TEnd
MEnd

Move DualWield_HighLow_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove        DualWield_Ready
	Flags		ReqInputs
	Scale		0.75

	Type		PraetorianClockwork_Victoria
	  Anim          male\DualBlade_Attack_HighLow 76
	TEnd
MEnd

############ readies #################################################

Move DualWield_Ready
	Requires	COMBAT WEAPON DUAL
#	Priority	3
#	Interrupts	<Ready> <combat> <WepCombat>
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle
	NextMove	postReady_back
	Scale		0.50

	Type		PraetorianClockwork_Victoria
	  Anim		male\dual_cycle	1 31
	TEnd
MEnd

Move WepReady
	Requires	COMBAT WEAPON
	Priority        2
	Interrupts	<Ready> <Combat>
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <WepCombat> <AttackIRQ> <MoveIRQ> <IRQEmotesRightHand>
	Flags		Cycle
	NextMove        postReady_back
	Scale           0.50

	Type		PraetorianClockwork_Victoria
	  Anim          male\wep_cycle 1 31
	TEnd
MEnd

Move CombatBlend
	Requires	COMBAT
	Priority        1
	Member		<DeathIRQ> <ReactIRQ> <HitIRQ> <Combat> <AttackIRQ> <MoveIRQ> <IRQEmotesCombat>
	Flags		ReqInputs
	NextMove        CombatReady
	Scale           0.50


	Type		PraetorianClockwork_Victoria
	  Anim          fem\combat_cycle 1 31
	TEnd

MEnd

## BOOSTDOWN ###########################################################

Move DualWield_BoostDown
	Requires	COMBAT WEAPON DUAL BOOSTDOWN
	Priority	4
	Interrupts	<AttackIRQ>
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	DualWield_BoostDown_PostCycle
	Flags		NoSizeScale

	Type		PraetorianClockwork_Victoria
	  Anim		fem\boostdown 1
	TEnd
MEnd

Move DualWield_BoostDown_PostCycle
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle NoSizeScale

	Type		PraetorianClockwork_Victoria
	  Anim          fem\Skel_ready2 1 60
	TEnd
MEnd

## 'BRAWL' PUNCH - RIGHT ###########################################################

Move Clockwork_Brawl_R
	Requires	COMBAT PUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Clockwork_Brawlpost_R
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksOnChild   PUNCH RIGHT

	Type		male
	  Anim          male\brawl_high2 1 25
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Clockwork_Brawlpost_R
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\brawl_high2 25 48
	TEnd
MEnd

## 'BRAWL' PUNCH - LEFT ###########################################################

Move Clockwork_Brawl_L
	Requires	COMBAT PUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Clockwork_Brawlpost_L
	Sets            CANTMOVE
	Flags		NoSizeScale
	SticksOnChild   PUNCH LEFT

	Type		male
	  Anim          male\brawl_high2 51 75
	  PlayFx        POWERS\POWERPUNCH\PUNCHWHOOSH2.FX
	TEnd
MEnd

Move Clockwork_Brawlpost_L
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\brawl_high2 75
	TEnd
MEnd

## Clockwork_CLAW SPIN ###########################################################

Move Clockwork_Claws_Spin_Draw
	Requires	COMBAT WEAPON DUAL CLAW CLAWAREA SPAWNSIDE
	Priority	56
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_Claws_Spin
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_out 1	19
	TEnd

	Type		huge
	  Anim		huge\claw_out 1	19
	TEnd

	Type		fem
	  Anim		fem\claw_out 1 19
	TEnd
MEnd

Move Clockwork_Claws_Spin
	Requires	COMBAT WEAPON DUAL CLAW CLAWAREA
	Priority	5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_Claws_Spin_Land
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_spin 1 32
	TEnd

	Type		huge
	  Anim		huge\claw_spin 1 32
	TEnd

	Type		fem
	  Anim		fem\claw_spin 1	32
	TEnd

MEnd

Move Clockwork_Claws_Spin_Land
	Requires	COMBAT
	Member		<DeathIRQ> <AttackIRQ>
	NextMove	Clockwork_Claws_Spin_Post
	Interpolate	0
	Sets		CANTMOVE
	Wind		ATTACK
	Flags		NoSizeScale

	Type		male
	  Anim		male\claw_spinpost 1 45
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		huge
	  Anim		huge\claw_spin 33 78
	  PlayFx	FEET\LAND1.FX  1
	TEnd

	Type		fem
	  Anim		fem\claw_spin 33 78
	  PlayFx	FEET\LAND1.FX  1
	TEnd

MEnd

Move Clockwork_Claws_Spin_Post
	Requires	COMBAT
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Clockwork_clawcycle_15
	Wind		ATTACK
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_spinpost 46
	TEnd

	Type		huge
	  Anim		huge\claw_spin 79
	TEnd

	Type		fem
	  Anim		fem\claw_spin 79
	TEnd

MEnd

#############################################################

Move Clockwork_ClawReady
	Requires	COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon.inc
	Flags		Cycle NoSizeScale
	NextMove	postReady_back
	Scale		0.50

	Type		male
	  Anim		male\claw_cycle	1
	TEnd

	Type		huge
	  Anim		huge\claw_cycle	1 31
	TEnd

	Type		fem
	  Anim		fem\claw_cycle 1 31
	TEnd

	Type		mako
	  Anim		male\claw_cycle_hands 1	31
	TEnd
MEnd

#############################################################

Move Clockwork_clawcycle_15
	Requires	COMBAT WEAPON DUAL CLAW
	Include sequencers\Members\Combat_DualWeapon.inc
	NextMove	Clockwork_ClawReady
	Scale		0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim		male\claw_cycle	15
	TEnd

	Type		huge
	  Anim		huge\claw_cycle	15 31
	TEnd

	Type		fem
	  Anim		fem\claw_cycle 15 31
	TEnd

MEnd

###################
###### Clockwork_Baseball punch ######
###################

Move Clockwork_BBpunch
	Requires	COMBAT BASEBALLPUNCH
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Clockwork_BBpunchpost
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\baseball_punch 1 44
	TEnd

	Type		huge
	  Anim          huge\baseball_punch 1 44
	TEnd

	Type		fem
	  Anim          fem\baseball_punch 1 44
	TEnd
MEnd

Move Clockwork_BBpunchpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Scale           0.50
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\baseball_punch 44
	TEnd

	Type		huge
	  Anim          huge\baseball_punch 44
	TEnd

	Type		fem
	  Anim          fem\baseball_punch 44
	TEnd
MEnd

###################
###### Clockwork_2Hand Cast  ######
###################

Move Clockwork_2HandCast
	Requires	COMBAT 2HAND CAST
	Priority        3
	Interrupts	<AttackIRQ> <Defiance> <Windups> Pre_WindUp WindUp
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Clockwork_2HandCastpost
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\2hand_cast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		huge
	  Anim          huge\2hand_cast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd

	Type		fem
	  Anim          fem\2hand_cast 1 50
	  PitchEnd	42
	  PitchAngle	0
	TEnd
MEnd

Move Clockwork_2HandCastpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\2hand_cast 50
	TEnd

	Type		huge
	  Anim          huge\2hand_cast 50
	TEnd

	Type		fem
	  Anim          fem\2hand_cast 50
	TEnd
MEnd

#####Clockwork_#######################################################

Move Clockwork_Rapid
	Requires	COMBAT RAPIDBLAST
	Priority        2
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Clockwork_Rapid_Post
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\energy_rapid 1 50
	TEnd

	Type		huge
	  Anim          huge\energy_rapid 1 50
	TEnd

	Type		fem
	  Anim          fem\energy_rapid 1 50
	TEnd
MEnd

Move Clockwork_Rapid_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\energy_rapid 50
	TEnd

	Type		huge
	  Anim          huge\energy_rapid 50
	TEnd

	Type		fem
	  Anim          fem\energy_rapid 50
	TEnd
MEnd

#############################################################

Move Clockwork_Rapid_Slow
	Requires	COMBAT RAPIDBLAST SLOW
	Priority        3
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Clockwork_Rapid_Slow_Post
	Flags		PITCHTOTARGET NoSizeScale
	Sets            CANTMOVE

	Type		male
	  Anim          male\energy_rapid 1 60
	TEnd
MEnd

Move Clockwork_Rapid_Slow_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Flags		NoSizeScale ReqInputs

	Type		male
	  Anim          male\energy_rapid 60
	TEnd
MEnd


###################
###### magic throw  ######
###################

Move clockwork_magicthrow
	Requires	COMBAT MAGICTHROW
	Priority        2
	NextMove        clockwork_magicthrowpost
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\frisbee 1 30
	  PitchAngle	0.3
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		huge
	  Anim          huge\frisbee 1 30
	  PitchStart	11
	  PitchEnd	23
	TEnd

	Type		fem
	  Anim          fem\frisbee 1 30
	  PitchStart	11
	  PitchEnd	23
	TEnd
MEnd

Move clockwork_magicthrowpost
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Combat_half
	Flags		NoSizeScale

	Type		male
	  Anim          male\frisbee 30 70
	TEnd

	Type		huge
	  Anim          huge\frisbee 30 70
	TEnd

	Type		fem
	  Anim          fem\frisbee 30 70
	TEnd
MEnd

###################
###### Clockwork_Poison Mode (MR.Spikey)  ######
###################

Move Clockwork_PrePoison
	Requires	COMBAT POISON
	Priority        2
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        Clockwork_PrePoison2
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 1 88
	TEnd

	Type		huge
	  Anim          huge\pre_poison 1 88
	TEnd

	Type		fem
	  Anim          fem\pre_poison 1 88
	TEnd
MEnd

Move Clockwork_PrePoison2
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        CombatReady
	Scale           0.50
	Flags		NoSizeScale

	Type		male
	  Anim          male\pre_poison 88
	TEnd

	Type		huge
	  Anim          huge\pre_poison 88
	TEnd

	Type		fem
	  Anim          fem\pre_poison 88
	TEnd
MEnd
