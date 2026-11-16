#########################################################
##	chill touch hit
FxInfo

LifeSpan	70

######################################################################

Condition
	On	Time
	Time	10

	Event
		EName	1
		Type	Local
		At	head
		part	:DroneHandGlows.part
		part	:BlueSparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Sound PeaceHands2 60 60 .7
		LifeSpan	40
	End

End


End				