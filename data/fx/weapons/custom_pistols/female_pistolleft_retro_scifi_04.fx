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
		Anim	CustomWeapon_PistolRight_Retro_SciFi_04
		BhvrOverride
			PositionOffset	0.15   -0.01     -0.08
			PYRRotate	0 	0 	  180
			Scale		0.8 	0.8  	  0.8
	End

	End
End

#############################################################

End