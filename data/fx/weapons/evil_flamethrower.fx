#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	Flamethrower
		Type	Local
		At	T1_L
		Anim	FX_turret_flamethrower_evil
	End
End

#############################################################

End