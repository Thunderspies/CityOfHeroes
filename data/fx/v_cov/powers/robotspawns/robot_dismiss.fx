
	
##########################################         RobotSpawn Drop         ###########################################


######################################################  NOTES   ######################################################
##
##	Overview: appears and drops from sky, jets emitting from feet.  Jets turn off, he drops, cloud fx on ground
##	
##	I found it easier to kill multiple events and keep things organized using Child Fx, so there are a
##	number of child fx calls here.  
##
######################################################################################################################

FxInfo

LifeSpan	150

Condition	
	On 	Time
	Time 	0

	Event
		EName	SpawnTarget
		Type	local
		At	Root
		
		Bhvr	behaviors\FX_RobotSpawn_DropTarget.bhvr
#		Geom	TESTING_Target
		Lifespan 150

	End	
End

#########################################################
####################  S o u n d s  ######################
#########################################################

### Activte Rocket ignition

Condition
	On	Time
	Time	30
	Event
		Type	Local					# Random robot startup sound
		At	Chest
		Sound Servodoor1 200 200 .79
	End
End



###  Activate trusters

Condition	
	On 	Time
	Time 	37

	Event
		EName	EngineSounds				# thruster jet
		Type	Local
		At	Hips
		Sound thruster_Loop 70 70 .3
	End

	
End

### lift off

Condition	
	On 	Time
	Time 	77

	Event
		EName	EngineSounds				# thruster jet
		Type	local
		At	Hips
		Sound arcoflame 70 70 .3
	End

	
End






##########################################################
#######################  J e t s  ########################
##########################################################

Condition	
	On 	Time
	Time 	80

	Event
		EName	JetRight
		Type	Start
		At	Hips
		ChildFX	:FX_RS_JetR.fx
		LifeSpan 70
	End
End


##### Other Leg  ####

Condition	
	On 	Time
	Time 	80

	Event
		EName	JetLeft
		Type	Start
		At	Hips
		ChildFX	:FX_RS_JetL.fx
		LifeSpan 70
	End
End



##########################################################
################## Ground Effect Jet ####################
##########################################################

Condition	
	On 	Time
	Time 	80

	Event
		EName	GroundFXL
		Type	Start
		At	FootL

		ChildFX	:RS_P_GroundJetL.fx
		Lifespan 15
	End
End



Condition	
	On 	Time
	Time 	80

	Event
		EName	GroundFXR
		Type	Start
		At	FootR
		ChildFX	:RS_P_GroundJetR.fx
		Lifespan 15
	End
End





#########################################################
################# Ground Effect dust ####################
#########################################################

####  Big Dust  ##################

Condition
	On 	Time
	Time 	79

	Event
		EName	Ring01
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust.part
		LifeSpan	20		
	End

End

Condition
	On 	Time
	Time 	81

	Event
		EName	Ring02
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
		LifeSpan	20		
	End

End

### little dust  ##################

Condition
	On 	Time
	Time 	45

	Event
		EName	Ring03
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust02.part
		LifeSpan	20		
	End

End

Condition
	On 	Time
	Time 	50

	Event
		EName	Ring04
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust03.part
		LifeSpan	20		
	End

End


Condition
	On 	Time
	Time 	60

	Event
		EName	Ring05
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
		LifeSpan	20		
	End

End


Condition
	On 	Time
	Time 	60

	Event
		EName	Ring05
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
		LifeSpan	20		
	End

End


Condition
	On 	Time
	Time 	65

	Event
		EName	Ring06
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
		LifeSpan	20		
	End

End


Condition
	On 	Time
	Time 	70

	Event
		EName	Ring07
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust01.part
		LifeSpan	10		
	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName	Ring08
		Type	Start
		At	Root
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRing.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDust02.part
		LifeSpan	7		
	End

End

## Feet Hit ##

Condition
	On 	Time
	Time 	80

	Event
		EName	Ring10
		Type	Local
		At	FootR
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRingEnd.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDustEnd01.part
		LifeSpan	1		
	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName	Ring10
		Type	Local
		At	FootL
		
		Bhvr	Behaviors/FX_RobotSpawn_DustRingEnd.bhvr
		Part	V_COV\Powers\RobotSpawns\RS_P_GroundHitDustEnd01.part
		LifeSpan	1		
	End

End


##################################################################################################################
##################################################################################################################
##################################################################################################################
##################################################################################################################
##################################################################################################################
##################################################################################################################
##################################################################################################################




#########################################################
#################  kill whats left  #####################
#########################################################

Condition
	On 	Time
	Time	150

	Event
		Ename 	All
		Type	Destroy
	End


End



End
















		