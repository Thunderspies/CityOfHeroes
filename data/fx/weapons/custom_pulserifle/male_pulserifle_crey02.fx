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
		#Anim	CustomWeapon_PulseRifle_03
		ChildFX	:FX_CreyLaserRifle_02.fx
	End
End

#############################################################

End