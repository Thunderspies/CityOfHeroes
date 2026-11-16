#########################################################
##	Fireweapon
########################################################
FxInfo

##################################################################################
################################## Right jet  ####################################
##################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EmitterR
		Type	local
		At	Back
		
		bhvr	ENEMYFX\V_GoldBrickers\Jetpack_P_JetRightEmit.bhvr
#		Geom	Testing_TargetB
	End

End

####################################################################################
##################################  particles  #####################################
####################################################################################

Condition
	On	Time
	Time	1

	Event
		EName 	JetR
		Type	local
		At	EmitterR	
		
#		Part1	ENEMYFX\V_Goldbrickers\GB_P_ThrustRingsEmit.part
		Part2	ENEMYFX\V_Goldbrickers\GB_P_ThrustFlareCore.part		
		Part3	ENEMYFX\V_Goldbrickers\GB_P_ThrustFlare.part
		PMagnet	JetR
	End

	Event
		EName	FlareRim
		Type	Local
		At	EmitterR
		BhvrOverride
			Behavior
			PositionOffset 0 .1 0
		End
		
		Part3	ENEMYFX\V_Goldbrickers\GB_P_ThrustFlareEmit.part
#		Part1	ENEMYFX\V_Goldbrickers\GB_P_ThrustRings.part
	End

	Event
		EName	FlareRim
		Type	Local
		At	EmitterR
		BhvrOverride
			Behavior
			PositionOffset 0 -.5 0
		End

#		Part1	ENEMYFX\V_Goldbrickers\Jetpack_turbine_Smoke02.part
	End
	
End

##################################################################################
################################# Left jet  #####################################
#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EmitterL
		Type	local
		At	Back
		BhvrOverride
			behavior
			PositionOffset		-.70 -.9 -.35
			Scale			1 1 1
		End

		bhvr	ENEMYFX\V_GoldBrickers\Jetpack_P_JetRightEmit.bhvr
#		Geom	Testing_TargetB
	End

End

###################################################################################
#################################  particles  #####################################
###################################################################################

Condition
	On	Time
	Time	1

	Event
		EName 	JetL
		Type	local
		At	EmitterL	
		
#		Part1	ENEMYFX\V_Goldbrickers\GB_P_ThrustRingsEmit.part
		Part2	ENEMYFX\V_Goldbrickers\GB_P_ThrustFlareCore.part		
		Part3	ENEMYFX\V_Goldbrickers\GB_P_ThrustFlare.part	
	End

	Event
		EName	JetLEmitRing
		Type	Local
		At	EmitterL
		BhvrOverride
			Behavior
			PositionOffset 0 .1 0
		End
		
		Part3	ENEMYFX\V_Goldbrickers\GB_P_ThrustFlareEmit.part
#		Part1	ENEMYFX\V_Goldbrickers\GB_P_ThrustRings.part
	End

	Event
		EName	JetLSmoke
		Type	Local
		At	EmitterL
		BhvrOverride
			Behavior
			PositionOffset 0 -.5 0
		End

#		Part1	ENEMYFX\V_Goldbrickers\Jetpack_turbine_Smoke02.part
	End
	
End


End

