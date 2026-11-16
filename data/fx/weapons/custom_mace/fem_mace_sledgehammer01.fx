#############################################################
## Costume Anim Entity - Mace - 02
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
		Bhvr	WEAPONS\Custom_Mace\Female_WepRScale.bhvr
		ChildFX	:FX_Sledgehammer_01.fx
	End
End

#############################################################

End