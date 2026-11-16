#########################################################
##	template

FxInfo

Lifespan 100


Condition
	On	Time
	Time	7
	
	Event
		EName	EnergyBallCore
		Type	Local
		At	Root
		Sound Powerup2 70 70 .8
	End
End

Condition
	On Time 
	Time 7
	
	Event
		EName	RightCore
		Type 	Local
		At	HandR
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
		At	HandR
		bhvrOverride
			PositionOffset	.2 0 0.02
		End
		part1	:P_Glow_RecluseHand_KnockUp.part
		part2	:P_GlowSparks_RecluseHand.part
	End

	Event
		EName	RightSparls
		Type 	Local
		At	F1_R
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseFingers_KnockUp.part
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
		
		part2	:P_Glow_RecluseHandMove_KnockUp01.part
	End
End

Condition
	On	Time
	Time	39
	
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

	