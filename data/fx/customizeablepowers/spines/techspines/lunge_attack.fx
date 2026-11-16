#############################################################
## Lunge_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 45

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak8
		Type	local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Grow.bhvr
		ChildFX	:Child_WepRSpine.fx
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Shrink.bhvr
		ChildFX	:Child_WepRSpine.fx
	End
End

#############################################################

End