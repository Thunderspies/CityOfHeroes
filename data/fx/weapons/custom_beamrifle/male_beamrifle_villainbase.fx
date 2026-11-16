#############################################################
## Costume Anim Entity - Pulse Rifle - Base
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
		#Anim	CustomWeapon_PulseRifle_VillainBase
		ChildFX	:FX_BeamRifle_VillainBase.fx
	End
End

#############################################################

End