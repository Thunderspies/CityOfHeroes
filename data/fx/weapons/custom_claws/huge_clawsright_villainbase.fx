#############################################################
## Costume Anim Entity - Right Claws - Base
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
		Bhvr	WEAPONS\Custom_Claws\Huge_WepRScale.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		Anim	CustomWeapon_ClawsRight_VillainBase
	End
End

#############################################################

End