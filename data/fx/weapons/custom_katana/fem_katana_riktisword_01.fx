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
		Bhvr	WEAPONS\Custom_Katana\Female_WepLScaleAndFlip.bhvr
		ChildFX	:FX_RiktiBlade_01.fx
	End
End

#############################################################

End