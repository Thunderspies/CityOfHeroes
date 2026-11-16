
	
##########################################         RobotSpawn Drop         ###########################################


##################################################################################################################

FxInfo





#########################################################
####################  S o u n d s  ######################
#########################################################






##########################################################
#######################  J e t s  ########################
##########################################################

Condition	
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName	JetRight
		Type	Start
		At	Hips
		While	FLY
		Flags	OneAtATime
		ChildFX	:Robot_Fly_JetR.fx

	End
End


##### Other Leg  ####

Condition	
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName	JetLeft
		Type	Start
		At	Hips
		While	FLY
		Flags	OneAtATime
		ChildFX	:Robot_Fly_JetL.fx

	End
End



##########################################################
################### Ground Effect Jet ####################
###########################################################
#
#Condition	
#	On 	Time
#	Time 	80
#
#	Event
#		EName	GroundFXL
#		Type	Start
#		At	FootL
#
#		ChildFX	:RS_P_GroundJetL.fx
#		Lifespan 15
#	End
#End
#
#
#
#Condition	
#	On 	Time
#	Time 	80
#
#	Event
#		EName	GroundFXR
#		Type	Start
#		At	FootR
#		ChildFX	:RS_P_GroundJetR.fx
#		Lifespan 15
#	End
#End
#
#
#
#
#
##########################################################
################## Ground Effect dust ####################
##########################################################
#
#####  Big Dust  ##################
#
#Condition
#	On 	Time
#	Time 	79
#
#	Event
#		EName	Ring01
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust.part
#		LifeSpan	20		
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	81
#
#	Event
#		EName	Ring02
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
#		LifeSpan	20		
#	End
#
#End
#
#### little dust  ##################
#
#Condition
#	On 	Time
#	Time 	45
#
#	Event
#		EName	Ring03
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust02.part
#		LifeSpan	20		
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	50
#
#	Event
#		EName	Ring04
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust03.part
#		LifeSpan	20		
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	60
#
#	Event
#		EName	Ring05
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
#		LifeSpan	20		
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	60
#
#	Event
#		EName	Ring05
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
#		LifeSpan	20		
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	65
#
#	Event
#		EName	Ring06
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
#		LifeSpan	20		
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	70
#
#	Event
#		EName	Ring07
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
#		LifeSpan	10		
#	End
#
#End
#
#Condition
#	On 	Time
#	Time 	80
#
#	Event
#		EName	Ring08
#		Type	Start
#		At	Root
#		
#		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
#		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust02.part
#		LifeSpan	7		
#	End
#
#End
#









End
















		