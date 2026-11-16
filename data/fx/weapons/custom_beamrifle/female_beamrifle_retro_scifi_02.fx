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
			PositionOffset	-0.010  -0      0.03
			Scale		0.675   0.675   0.675
			PyrRotate 	0       -1       3
	End
End

#############################################################

End