#############################################################
## Costume Anim Entity - Pulse Rifle - 01
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
		Bhvr	WEAPONS\Custom_PulseRifle\Female_WepRScale.bhvr
		ChildFX	:FX_CreyLaserRifle_02.fx
	End
End

#############################################################

End