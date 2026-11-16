#############################################################
## CustomWeapon_PulseRifle.fx
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
		Sound shotgunout 40 40 .3
	End
End

#############################################################

End