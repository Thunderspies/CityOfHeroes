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
		Part	ENEMYFX\Ghouls\Inspiration_Bracelet_Glow.part
		Part	ENEMYFX\Ghouls\Inspiration_General_Glow.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	ENEMYFX\Ghouls\Inspiration_Bracelet_Glow.part
		Part	ENEMYFX\Ghouls\Inspiration_General_Glow.part
		Lifespan	10
	End
End
#############################################################

End