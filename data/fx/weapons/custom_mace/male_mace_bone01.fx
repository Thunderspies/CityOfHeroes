#############################################################
## Costume Anim Entity - Mace - 09
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
		#Anim	CustomWeapon_Mace_04
		ChildFX	:FX_GiantBone_01.fx
	End
End

#############################################################

End