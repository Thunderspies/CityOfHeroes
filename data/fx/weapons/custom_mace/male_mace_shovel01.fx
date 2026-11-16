#############################################################
## Costume Anim Entity - Mace - 05
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
		BhvrOverride
			PyrRotate	0 0 115
		End
		#Anim	CustomWeapon_Mace_04
		ChildFX	WEAPONS\Custom_Axe\FX_Shovel_01.fx
	End
End

#############################################################

End