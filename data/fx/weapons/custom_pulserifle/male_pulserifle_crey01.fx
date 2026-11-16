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
		#Anim	CustomWeapon_PulseRifle_02
		ChildFX	:FX_CreyLaserRifle_01.fx
	End
End

#############################################################

End