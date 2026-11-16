#############################################################
## Costume Anim Entity - Left Pistol - 03
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
		Bhvr	Behaviors\Reverse_Wep.bhvr
		ChildFX	:FX_PistolRight_TacticalSidearm_LaserSight.fx
	End
End

#############################################################

End