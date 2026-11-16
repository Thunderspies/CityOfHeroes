#############################################################
## Trick Arrow - Acid Arrow (Child)
#############################################################

FxInfo

ClampMaxScale 1 1 1

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Origin
		Part	:Acid.part
		Part	:AcidYellow.part
		LifeSpan 12
	End

	Event
		EName 	arrow
		Type	Start
		At	Origin
		Part	:AcidFlat.part
		Part	:AcidFlatDark.part
		Part	:AcidBubbles.part
		Part	:AcidBubblesdark.part
		Part	:AcidBubble.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On	Cycle
	Time	1

	Event
		Type	Start
		At	Origin
		bhvr	:AcidSplatGas.bhvr
		Splat	AcidCloud.tga
		LifeSpan 11
	End
End

Condition
	On	Cycle
	Time	1

	Event
		Type	Start
		At	Origin
		bhvr	:AcidSplatGasDark.bhvr
		Splat	AcidCloud.tga
		LifeSpan 11
	End
End

Condition
	On	Cycle
	Time	1

	Event
		Type	Start
		At	Origin
		bhvr	:AcidSplatDropsRing.bhvr
		Splat	Generic_Ring
		LifeSpan 12
	End
End

Condition
	On	Cycle
	Time	2

	Event
		Type	Start
		At	Origin
		bhvr	:AcidSplatDropsRingdark.bhvr
		Splat	Generic_Ring
		LifeSpan 12
	End
End

#############################################################

End