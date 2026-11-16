#########################################################
##	template

FxInfo

Lifespan 100

Condition
	On Time 
	Time 0
	
	Event
		EName	RightCore
		Type 	Local
		At	HandR
		bhvrOverride
			PositionOffset	.2 0 0.02
		End		
		part3	:P_GlowCore_RecluseHand.part
#		Geom	Testing_TargetB
	End
	
End

Condition
	On	Time
	Time	5

	Event
		EName	RightGlow
		Type 	Local
		At	HandR
		bhvrOverride
			PositionOffset	.2 0 0.02
		End
		part1	:P_Glow_RecluseHand.part
		part2	:P_GlowSparks_RecluseHand.part
	End

	Event
		EName	RightSparls
		Type 	Local
		At	F1_R
		part1	:P_Glow_RecluseFingers.part
	End

End

Condition
	On	Time
	Time	14
	
	Event
		EName	Trail
		Type	Local
		At	RightCore
		
		part2	:P_Glow_RecluseHandMove.part
	End
End




End	

	