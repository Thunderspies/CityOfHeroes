#############################################################
## Costume Anim Entity - Pulse Rifle - 02
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
		Bhvr	WEAPONS\Custom_PulseRifle\Female_WepRScale.bhvr
		ChildFX	:FX_RiktiRifle_01.fx
	End
End

#############################################################

End