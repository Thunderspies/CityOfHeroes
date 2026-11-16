#############################################################
## Costume Anim Entity - Axe - 04
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Axe\Female_WepRScale.bhvr
		ChildFX	:FX_RiktiAxe_01.fx
	End
End

#############################################################

End