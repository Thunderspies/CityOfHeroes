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
		Part	:RiseToTheChallenge_LargeGlow1.part
		Part	:RiseToTheChallenge_LargeStar1.part
		sound	heart_loop 50 40 .21
		bhvr	behaviors/soundFade1.bhvr
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
		Part	:RiseToTheChallenge_LargeGlow2.part
		Part	:RiseToTheChallenge_LargeStar2.part
		Lifespan	60
	End
End

#########################################################

End			