#############################################################
## Costume Anim Entity - Mace - 08
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
		#Anim	CustomWeapon_Mace_04
		ChildFX	:FX_BaseballBat_01.fx
	End
End

#############################################################

End