#############################################################
## Costume Anim Entity - Right Pistol - Base
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
		Anim	CustomWeapon_PistolRight_Diamondback
		BhvrOverride
			Scale			0.8 0.8 0.8
			PositionOffset		0 0 -0.02
		End
	End
End

#############################################################

End