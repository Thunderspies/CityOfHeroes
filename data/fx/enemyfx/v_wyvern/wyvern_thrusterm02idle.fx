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
			PositionOffset		0 -.45 -.5
		End
		bhvr	ENEMYFX\V_Wyvern\Wyvern_B_EmitterMid.bhvr
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
		BhvrOverride
			behavior
			FadeOutLength	130
			Scale		.5 .5 .5
			
		End
		
		Part1	ENEMYFX\V_Wyvern\Wyvern_P_ThrustRingsEmitIdle.part
#		Part2	ENEMYFX\V_Wyvern\Wyvern_P_ThrustFlareCore.part
#		Part3	ENEMYFX\V_Wyvern\Wyvern_P_ThrustFlare.part
		Sound glow7_loop 70 70 .75
	End

	Event
		EName 	JetR
		Type	local
		At	Emitter		
		BhvrOverride
			Behavior
			PositionOffset	0 0.1 0
			FadeOutLength	130
			Scale		.5 .5 .5
		End

		Part3	ENEMYFX\V_Wyvern\Wyvern_P_ThrustRingsEmit01.part
	End

	Event
		EName	FlareRim
		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -.15 0
			FadeOutLength	130
		End

#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRings.part
	End

	Event
		EName	FlareRim
		Type	Local
		At	Emitter
		BhvrOverride
			Behavior
			PositionOffset 0 -.5 0
			FadeOutLength	130
		End

#		Part1	ENEMYFX\V_LongBow\LB_P_Smoke.part
	End
	
End

End
