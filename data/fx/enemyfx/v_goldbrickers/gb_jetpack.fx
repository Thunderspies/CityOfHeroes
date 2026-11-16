FXInfo

###############################################################################################
########################################   XXXXX    ###########################################
###############################################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EmitterL
		Type	local
		At	Back
		BhvrOverride
			behavior
			PositionOffset		-.70 -.9 -.35
			Scale			1 1 1
		End
		
#		Geom	Testing_TargetB
	End

	Event
		EName 	EmitterR
		Type	local
		At	Back
		
		bhvr	ENEMYFX\V_GoldBrickers\Jetpack_P_JetRightEmit.bhvr
#		Geom	Testing_TargetB
	End
End

###############################################################################################
########################################   XXXXX    ###########################################
###############################################################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Local 
		At	chest
		Geom	proplink			
	End
End


Condition
	On		TriggerBits
	TriggerBits	
	DoMany		1

	Event
		EName 	Propeller
		Type	Local 
		At	Prime
		 AltPiv 1
		Until	FLY
		Flags	OneAtATime

		##Bhvr	enemyfx/v_goldbrickers/Jetpack_Propeller_Idle.bhvr
		Bhvr	enemyfx/v_goldbrickers/Jetpack_Propeller_spin.bhvr
		Geom	Propeller2
	End

	Event
		EName	SmokeL
		Type	Local
		At	EmitterL
		Until	FLY
		Flags	OneAtATime

		Part1	ENEMYFX\V_Goldbrickers\Jetpack_turbine_Smoke02.part
	End

	Event
		EName	SmokeR
		Type	Local
		At	EmitterR
		Until	FLY
		Flags	OneAtATime

		Part1	ENEMYFX\V_Goldbrickers\Jetpack_turbine_Smoke02.part
	End

End

###############################################################################################
########################################   Hover    ###########################################
###############################################################################################


###############################################################################################
########################################   In Air   ###########################################
###############################################################################################


Condition
	On	TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	ChildThrust
		Type	local
		At	Chest
		While	FLY 
		Flags	OneAtATime

		ChildFX	ENEMYFX\V_Goldbrickers\jetpack_turbine.fx
		Sound Thruster_loop 50 50 .3
	End
End

End