#############################################################
## FX_Nova_Tail.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Tail_1
		Type	Local
		At	ToeL
		Anim	FX_Nova_Tail
		Bhvr	Behaviors/Nova_tail_1.bhvr
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName	Tail_2
		Type	Local
		At	ToeL
		Anim	FX_Nova_Tail
		Bhvr	Behaviors/Nova_tail_2.bhvr
	End
End

#############################################################

End