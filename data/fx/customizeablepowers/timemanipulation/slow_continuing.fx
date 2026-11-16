#############################################################
## Slow_Conditional.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_SlowedResponse_Continuing_Loop 100 100 .15
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End


Condition
	On	Time
	Time	0

	Event
		EName	SwirlAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 4.0 0.0
		End
	End

	Event
		Type	PositOnly
		At	SwirlAnchor
		Part	:Swirl_Slow_Core_Additive_Burst.part
		Part	:Swirl_Slow_Core_Burst.part
		Part	:Swirl_Slow_Highlights.part
		Lifespan 90
	End
End


Condition
	On	Cycle
	Time	120

	Event
		Type	PositOnly
		At	SwirlAnchor
		Part	:Swirl_Slow_Core_Additive_Burst.part
		Part	:Swirl_Slow_Core_Burst.part
		Part	:Swirl_Slow_Highlights.part
		Lifespan 90
	End
End

#############################################################

End