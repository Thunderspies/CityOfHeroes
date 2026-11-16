#############################################################
## Header
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha

#############################################################

Condition
	On 	cycle
	Time 	5
	Chance	0.33

	Event
		Type	Local
		At	Origin
		Part	:EMPFlash.part
		Part	:EMPFlashRing.part
		Part	:EMPFlashStars.part
		LifeSpan	5
	End
End