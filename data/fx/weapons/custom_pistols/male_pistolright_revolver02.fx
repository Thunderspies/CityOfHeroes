#############################################################
## Costume Anim Entity - Right Pistol - 03
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
		#Anim	CustomWeapon_PistolRight_03
		ChildFX	:FX_PistolRight_Revolver_02.fx
	End
End

#############################################################

End