#############################################################
## Costume Anim Entity - Broadsword 06
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		ChildFX	:FX_Katana_01.fx
	End
End

#############################################################

End