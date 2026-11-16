#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################

FxInfo

Flags InheritAlpha ##InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Anim	CustomWeapon_BeamRifle_Retro_SciFi_02
		BhvrOverride
			PositionOffset	-0.1  	-0      0
			Scale		1   	1   	1
			PyrRotate 	3       -05    0
	End
End

#############################################################

End