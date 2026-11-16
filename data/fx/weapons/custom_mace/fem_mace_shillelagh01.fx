#############################################################
## Costume Anim Entity - Mace - 09
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
		ChildFX	:FX_Shillelagh_01.fx
	End
End

#############################################################

End