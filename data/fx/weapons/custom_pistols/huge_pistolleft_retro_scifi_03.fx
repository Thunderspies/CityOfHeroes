#############################################################
##  Costume Anim Entity - Left Pistol - 02
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\Reverse_Wep.bhvr
		Anim	CustomWeapon_PistolRight_Retro_SciFi_03
		BhvrOverride
			PositionOffset	0.05  	-0     	-0.05
			Scale		1.2    1.2     	1.2
			PyrRotate 	0      3     	180
	End
End

#############################################################

End