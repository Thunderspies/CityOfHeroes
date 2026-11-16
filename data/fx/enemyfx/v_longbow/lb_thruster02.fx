#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Emitter
		Type	local
		At	Chest
		BhvrOverride
			Behavior
			PositionOffset		0 -.47 -.7
		End
		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterMid.bhvr
#		Geom	Testing_TargetB
	End

End

#####################################################################################
################################### center jet  #####################################
#####################################################################################

Condition
	On	Time
	Time	1

	Event
		EName 	JetR
		Type	local
		At	Emitter		
		
		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit.part
		Part2	ENEMYFX\V_LongBow\LB_P_ThrustFlareCore.part
		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare.part
		Sound glow7_loop 70 70 .66
	End

	Event
		EName	FlareRim
		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -.15 0
		End

		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRings.part
	End

	Event
		EName	FlareRim
		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -.5 0
		End

		Part1	ENEMYFX\V_LongBow\LB_P_Smoke.part
	End
	
End

