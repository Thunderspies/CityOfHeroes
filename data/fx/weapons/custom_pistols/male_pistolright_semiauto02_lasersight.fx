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
		ChildFX	:FX_PistolRight_SemiAuto_03_LaserSight.fx
	End
End

#############################################################

End