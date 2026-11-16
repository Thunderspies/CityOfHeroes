#############################################################
## Costume Anim Entity - Axe - 03
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
		ChildFX	:FX_Hatchet_01.fx
	End
End

#############################################################

End