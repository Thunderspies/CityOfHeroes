#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Grow.bhvr
		ChildFX	:Child_WepLSpine.fx
		Lifespan 60
	End

	Event
		Type	local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Grow.bhvr
		ChildFX	:Child_WepRSpine.fx
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	60

	Event
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Shrink.bhvr
		ChildFX	:Child_WepLSpine.fx
	End

	Event
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Shrink.bhvr
		ChildFX	:Child_WepRSpine.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		Sound Fast6 80.0 80.0 0.8
	End
End

Condition
	On 	Time
	Time 	48

	Event
		Type	Start
		At	Chest
		Part	CustomizeablePowers\Spines\Spines\EviscerateStreak.part
		Lifespan 3
	End
End

#############################################################

End