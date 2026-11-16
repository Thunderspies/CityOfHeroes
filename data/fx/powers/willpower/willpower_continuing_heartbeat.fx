#########################################################
##  Willpower - Continuing Effect - Heartbeat (Child)
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	:WillPower_Continuing_LargeGlow1.part
		Part	:WillPower_Continuing_LargeStar1.part
		#sound	heart_loop 50 40 .21
		#bhvr	behaviors/soundFade3.bhvr
		Lifespan	60
	End
End

Condition
	On	Time
	Time	13
	
	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	:WillPower_Continuing_LargeGlow2.part
		Part	:WillPower_Continuing_LargeStar2.part
		Lifespan	60
	End
End

#########################################################

End			