#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	7

	Event
		EName 	arrow
		Type	start
		At	root
		Bhvr	:SonicRingWhite.bhvr
		Splat	Generic_Ring.tga
		Part	:SonicCoreRings.part
		Part	:SonicRingsOut2.part
		LifeSpan	15
	End

	Event
		EName 	arrow
		Type	start
		At	root
		Bhvr	:SonicRingDark.bhvr
		Splat	thinRing
		LifeSpan 15
	End
End

#############################################################

End