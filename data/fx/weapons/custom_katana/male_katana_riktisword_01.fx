#############################################################
## Costume Anim Entity - Broadsword 05
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors/Reverse_Wep.bhvr
		ChildFX	:FX_RiktiBlade_01.fx
	End
End

#############################################################

End