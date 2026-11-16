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
		Bhvr	WEAPONS\Custom_Pistols\Huge_WepRScale.bhvr
		Anim	CustomWeapon_PistolRight_SuperPack2_01
		BhvrOverride
			Scale			1.3 1.3 1.3
			PositionOffset		0.04 0 0
		End
	End
End

#############################################################

End