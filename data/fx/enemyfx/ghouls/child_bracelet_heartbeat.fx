#############################################################
## EnduranceHands.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\Ghouls\Bracelet_Glow.part
		Lifespan	60
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\Ghouls\Bracelet_Glow.part
		Lifespan	60
	End
End
#############################################################

End