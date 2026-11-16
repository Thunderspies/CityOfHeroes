#############################################################
## AnimatedTail_Demon.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Bhvr	:Huge_Scale.bhvr
		Anim	FX_PlayerTail_Rodent01
	End
End

#############################################################

End