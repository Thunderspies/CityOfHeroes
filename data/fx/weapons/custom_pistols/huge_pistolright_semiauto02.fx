#############################################################
## Costume Anim Entity - Right Pistol - 03
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
		Bhvr	WEAPONS\Custom_Pistols\Huge_WepRScale.bhvr
		ChildFX	:FX_PistolRight_SemiAuto_03.fx
	End
End

#############################################################

End