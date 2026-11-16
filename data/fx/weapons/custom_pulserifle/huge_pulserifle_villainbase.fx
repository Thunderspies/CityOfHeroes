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
		Bhvr	WEAPONS\Custom_PulseRifle\Huge_WepRScale.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		ChildFX	:FX_PulseRifle_VillainBase.fx
	End
End

#############################################################

End