FxInfo


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
			PositionOffset	-.75 -.55 -.215
		End


		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterLeft.bhvr
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

		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsSmall.part
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
			PositionOffset	.75 -.55 -.215
		End

		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterRight.bhvr
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

		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsSmall.part
	End
End

End