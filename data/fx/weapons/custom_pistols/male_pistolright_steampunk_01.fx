#############################################################
## Costume Anim Entity - Right Pistol - 02
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
		Anim	CustomWeapon_PistolRight_Steampunk_01
		#ChildFX	:FX_PistolRight_Revolver_01.fx
	End
End

#############################################################

End