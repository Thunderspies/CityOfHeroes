#############################################################
## FX_Nova_Jaw.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Jaw
		Type	Local
		At	Head
		Anim	FX_Nova_Jaw
	End
End

#############################################################

End