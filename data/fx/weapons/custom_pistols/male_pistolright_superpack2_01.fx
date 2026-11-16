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
		Anim	CustomWeapon_PistolRight_SuperPack2_01
#		BhvrOverride
#			Scale			1 1 1
#			PositionOffset		-0.1 0 0
#		End
	End
End

#############################################################

End