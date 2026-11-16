#############################################################
## Costume Anim Entity - Left Pistol - 03
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
		Bhvr	Behaviors/Reverse_Wep.bhvr
		#Anim	CustomWeapon_PistolRight_03
		ChildFX	:FX_PistolRight_Revolver_03.fx
	End
End

#############################################################

End