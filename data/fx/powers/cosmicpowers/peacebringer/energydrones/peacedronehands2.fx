#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	70

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	60
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	1
		Type	Local
		At	WepR
		part	:DroneHandGlows.part
		part	:BlueSparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		Sound PeaceHands2 60 60 .7
		LifeSpan	40
	End

	Event
		EName	1
		Type	Local
		At	WepL
		part	:DroneHandGlows.part
		part	:BlueSparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	40
	End

End

#############################################################

End				