FxInfo


Condition
	On 	Time
	Time 	0

	Event
		EName 	Reference
		Type	local
		At	Chest

		bhvr	ENEMYFX\V_Wyvern\Wyvern_B_EmitterMid.bhvr
#		Geom	Testing_TargetB
	End

End


#####################################################################################
##################################    Left jet    ###################################
#####################################################################################


Condition
	On	Time
	Time	1

	Event
		EName	LeftEmitter
		Type	Local
		At	Reference
		BhvrOverride
			behavior
			Scale		.35 .35 .35
		End

		bhvr	ENEMYFX\V_Wyvern\Wyvern_B_EmitterLeft.bhvr
#		Geom	Testing_targetB
	End
End

Condition
	On	Time
	Time	2

	Event	
		EName	JetLeft
		Type	Local
		At	LeftEmitter

		Part1	ENEMYFX\V_Wyvern\Wyvern_P_ThrustRingsEmitIdle.part
#		Part2	ENEMYFX\V_Wyvern\Wyvern_P_ThrustFlareCore.part
#		Part3	ENEMYFX\V_Wyvern\Wyvern_P_ThrustFlare.part
	End

	Event
		EName 	JetL
		Type	local
		At	LeftEmitter
		BhvrOverride
			behavior
			PositionOffset	0 0.1 -.05
		End

		Part3	ENEMYFX\V_Wyvern\Wyvern_P_ThrustRingsEmit01.part
	End
End

#####################################################################################
##################################    Right jet    ###################################
#####################################################################################



Condition
	On	Time
	Time	1
	
	Event	
		EName	RightEmitter
		Type	Local
		At	Reference
		BhvrOverride
			behavior
			Scale	.35 .35 .35
		End

		bhvr	ENEMYFX\V_Wyvern\Wyvern_B_EmitterRight.bhvr
#		Geom	Testing_targetB
	End
End	

Condition
	On	Time
	Time	2

	Event	
		EName	JetRight
		Type	Local
		At	RightEmitter

		Part1	ENEMYFX\V_Wyvern\Wyvern_P_ThrustRingsEmitIdle.part
#		Part2	ENEMYFX\V_Wyvern\Wyvern_P_ThrustFlareCore.part
#		Part3	ENEMYFX\V_Wyvern\Wyvern_P_ThrustFlare.part
	End

	Event
		EName 	JetR
		Type	local
		At	RightEmitter
		BhvrOverride
			behavior
			PositionOffset	0 0.1 -.05
		End

		Part3	ENEMYFX\V_Wyvern\Wyvern_P_ThrustRingsEmit01.part
	End
End

End