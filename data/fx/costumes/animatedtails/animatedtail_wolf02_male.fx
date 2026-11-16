#############################################################
## AnimatedTail_Wolf02.fx
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
		Anim	FX_PlayerTail_Wolf02
	End
End


#############################################################

End