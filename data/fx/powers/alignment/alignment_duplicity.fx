#############################################################
## LeadershipTactics.fx
#############################################################

FxInfo
Lifespan 45

##################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Rings_Spinning.part
		Part	:Rings_Spinning_Additive.part
		Part	:Smoke_Expanding.part
		Part	:Glow_Smaller_Continuing.part
		Lifespan 	40
		LifespanJitter	5
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Rings_Spinning.part
		Part	:Rings_Spinning_Additive.part
		Part	:Smoke_Expanding.part
		Part	:Glow_Smaller_Continuing.part
		Lifespan 	40
		LifespanJitter	5
	End

End