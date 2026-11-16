#############################################################
## AnimatedTail_Wolf.fx
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
		Anim	FX_PlayerTail_Wolf01
	End
End

#############################################################

End