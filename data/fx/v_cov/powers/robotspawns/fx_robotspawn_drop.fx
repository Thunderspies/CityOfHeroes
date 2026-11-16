
	
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

LifeSpan	200

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

Condition	
	On 	Time
	Time 	0

	Event
		EName	EngineSounds				# thruster jet
		Type	Start
		At	Hips

		Sound thruster_Loop 70 70 .3
	End

	Event
		EName	EngineSounds01				# thruster hum
		Type	Start
		At	Hips

		
		Sound glow7_loop 70 70 .66
	End
	
End


## hit ground ##

Condition
	On	Time
	Time	72

	Event
		EName	FootRSound				# first foot impacting ground
		Type	Local
		At	FootR
		bhvr	behaviors\FX_RobotSpawn_CamShake.bhvr
		Sound giantfoot1 85 85 .68
	End

End

Condition
	On	Time
	Time	76

	Event
		EName	FootLSound				# second foot impacting ground
		Type	Local
		At	FootL
		bhvr	behaviors\FX_RobotSpawn_CamShake01.bhvr
		Sound giantfoot1 85 85 .68
	End

End

Condition
	On	Time
	Time	83
	Event
		Type	Local					# Random robot startup sound
		At	Chest
		Sound Servodoor1 200 200 .79
	End
End



##########################################################
#######################  J e t s  ########################
##########################################################

Condition	
	On 	Time
	Time 	0

	Event
		EName	JetRight
		Type	Start
		At	Hips

		ChildFX	:FX_RS_JetR.fx
	End
End


Condition
	On 	Time
	Time	64

	Event
		Ename 	JetRight
		Type	Destroy
	End
End

##### Other Leg  ####

Condition	
	On 	Time
	Time 	0

	Event
		EName	JetLeft
		Type	Start
		At	Hips

		ChildFX	:FX_RS_JetL.fx
	End
End


Condition
	On 	Time
	Time	65

	Event
		Ename 	JetLeft
		Type	Destroy
	End
End

##########################################################
################## Ground Effect Jet ####################
##########################################################

Condition	
	On 	Time
	Time 	30

	Event
		EName	GroundFXL
		Type	Start
		At	FootL

		ChildFX	:RS_P_GroundJetL.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time	66

	Event
		Ename 	GroundFXL
		Type	Destroy
	End
End


Condition	
	On 	Time
	Time 	30

	Event
		EName	GroundFXR
		Type	Start
		At	FootR

		ChildFX	:RS_P_GroundJetR.fx
	End
End

Condition
	On 	Time
	Time	65

	Event
		Ename 	GroundFXR
		Type	Destroy
	End
End



#########################################################
################# Ground Effect dust ####################
#########################################################

Condition	
	On 	Time
	Time 	0

	Event
		EName	GroundFX
		Type	Start
		At	root

		ChildFX	:RS_P_GroundEffects.fx
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
















		