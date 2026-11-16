
#########################################################################################
########	Notes:
########
#########################################################################################

FxInfo
	
LifeSpan	90

################################################################################
###########################  Emitter points  ###################################
################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	TargetDummy
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 4 75
		End

####		Geom	Testing_TargetB
	End

	Event
		EName	EmitterPos
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 4.0 5.0
		End
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltEmitter.bhvr
#		Geom	Testing_Target
	End
End

#################################################################################
###########################  P R I M E   B U R S T  ###################  01  ####
#################################################################################

#Condition
#	On	Time
#	Time	2
#
#	Event
#		EName	PrimeSplat
#		Type	Start
#		At	EmitterPos
#		
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst01.part
#		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst.part
#		#PMagnet	T_chest
#	End
#End
#

#################################################################################
################################  P R I M E  ##########################  01  ####
#################################################################################

Condition
	On	Time
	Time	2

	Event
		EName	Prime
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\B_Projectile_ConeTravel.bhvr
#		Geom	Testing_Target
		Magnet	TargetDummy
		LookAt	TargetDummy
	End

############  spinning cloud Horozontal target 01

	Event
		EName	OffsetAlt01	# offset node that orbits projectile
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	10 0 0 
		End
#		Geom	Testing_Target
	End

###########  spinning cloud target Horizontal 02

	Event
		EName	OffsetAlt02	# offset node that orbits projectile
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	-10 0 0
		End
#		Geom	Testing_Target
	End

###########  spinning cloud target Horizontal 02

	Event
		EName	OffsetAlt03	# offset node that orbits projectile
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0 5 0
		End
#		Geom	Testing_Target
	End

End



#################################################################################
#################################  Bursts  ############################  01  ####
#################################################################################

Condition
	On	Time
	Time	2

	Event
		EName	splatRight
		Type	PositOnly
		At	OffsetAlt01

		Part1	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloudStreaks.part
		Part2	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloud.part
		Part3	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_LightSpecks.part
		Part4	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_DarkSpecks.part
		PMagnet	OffsetAlt01 #T_Chest
		POther	OffsetAlt02
	End

	Event
		EName	SplatLeft
		Type	PositOnly
		At	OffsetAlt02

		Part2	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloudStreaks.part
		Part3	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloud.part
		Part3	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_LightSpecks.part
		Part4	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_DarkSpecks.part
		PMagnet	OffsetAlt02  #T_Chest
		POther	OffsetAlt01
	End

	Event
		EName	SplatCenter
		Type	PositOnly
		At	Prime

		Part2	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloudStreaks.part
		Part3	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloud.part
		PMagnet	Prime  #T_Chest
#		POther	OffsetAlt01
	End

	Event
		EName	SplatTop
		Type	PositOnly
		At	OffsetAlt03

		Part2	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloudStreaks.part
		Part3	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_RedCloud.part
		Part3	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_LightSpecks.part
		Part4	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_DarkSpecks.part
		PMagnet	OffsetAlt03  #T_Chest
		POther	Prime
	End

#############
End

Condition
	On	Time
	Time	4

	Event
		EName	SplatBurst
		Type	Local
		At	Prime
		
		Part1	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Energy_LightCloudBurst.part
		PMagnet	SplatTop
	End

End

End













