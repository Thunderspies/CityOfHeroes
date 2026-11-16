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
		Anim	CustomWeapon_PistolRight_Retro_SciFi_02
		BhvrOverride
			PositionOffset	0.05  	-0     	-0.05
			Scale		1.2    1.2     	1.2
			PyrRotate 	0      3     	-1
	End
End

#############################################################

End