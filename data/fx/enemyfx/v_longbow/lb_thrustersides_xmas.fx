FxInfo

Flags InheritAlpha DontSuppress

#####################################################################################
##################################    Left jet    ###################################
#####################################################################################


Condition
	On	Time
	Time	1

	Event
		EName	LeftEmitter
		Type	Local
		At	Chest
				BhvrOverride
			Behavior
			PositionOffset -.76 -.55 -.25
		End

		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterLeft.bhvr

	End
End

Condition
	On	Time
	Time	2

	Event
		EName	JetLeft
		Type	Local
		At	LeftEmitter

		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsSmall_XMAS.part
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
		At	Chest
				BhvrOverride
			Behavior
			PositionOffset .76 -.55 -.25
		End

		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterRight.bhvr

	End
End

Condition
	On	Time
	Time	2

	Event
		EName	JetRight
		Type	Local
		At	RightEmitter

		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsSmall_XMAS.part
	End
End

End