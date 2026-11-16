#############################################################
## Beaker.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	RightFlask
		Type	Local
		At	WepR
		Geom	GEO_ErlenmeyerFlask_01
		sound	Emote_SSDrinkFormula 200 190 1
		Lifespan 76
	End
End

Condition
	On	Time
	Time	40

	Event
		EName 	Beaker
		Type	PositOnly
		At	RightFlask
		AltPiv	1
		Part	:LiquidPour.part
		POther	Head
		Lifespan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	86

	Event
		Type	Local
		At	Mystic
		Geom	GEO_ErlenmeyerFlask_01
		Lifespan 2
	End
End

Condition
	On 	Time
	Time 	88

	Event
		EName 	BeakerSmash
		Type	Start
		At	Mystic
		ChildFX	:Child_BeakerShatter.fx
	End
End

#############################################################

End