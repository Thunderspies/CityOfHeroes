#############################################################
##  Costume Anim Entity - Right Pistol - Base
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
		Anim	CustomWeapon_PistolRight_Retro_SciFi_04
		BhvrOverride
			PositionOffset	-0.010  0.01      -0.08
			Scale		0.8 	0.8  	0.8
			PyrRotate 	0       0       0

	End
End

#############################################################

End