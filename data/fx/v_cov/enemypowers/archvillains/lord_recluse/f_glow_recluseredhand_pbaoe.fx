#########################################################
##	template

FxInfo

Lifespan 130




Condition
	On Time 
	Time 7
	
	Event
		EName	RightCore
		Type 	Local
		At	Origin
		bhvrOverride
			PositionOffset	.2 0 0.02
			HueShift	15
		End		
		part3	:P_GlowCore_RecluseHand.part
#		Geom	Testing_TargetB
	End
	
End

Condition
	On	Time
	Time	12

	Event
		EName	RightGlow
		Type 	Local
		At	Origin
		bhvrOverride
			PositionOffset	.2 0 0.02
		End
		part1	:P_Glow_RecluseHand_PBAoE.part
		part2	:P_GlowSparks_RecluseHand_PBAoE.part
	End

	Event
		EName	RightSparls
		Type 	Local
		At	F1_R
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseFingers_PBAoE.part
	End

End

Condition
	On	Time
	Time	10
	
	Event
		EName	TrailShort
		Type	Local
		At	RightCore
		bhvrOverride
			HueShift	10
		End
		
		part2	:P_Glow_RecluseHandMove_PBAoE.part
	End
End

Condition
	On	Time
	Time	45
	
	Event
		EName	Trail
		Type	Local
		At	RightCore
		bhvrOverride
			HueShift	7
		End
		
		part2	:P_Glow_RecluseHandMove_KnockUp.part
	End
End




End	

	