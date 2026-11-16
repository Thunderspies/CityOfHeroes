#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	60

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
	Time	15

	Event
		EName	1
		Type	Local
		At	WepR
		part	:DroneHandGlows.part
		part	:BlueSparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	50
	End

	Event
		EName	1
		Type	Local
		At	WepL
		part	:DroneHandGlows.part
		part	:BlueSparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	50
	End
End

#############################################################

End				