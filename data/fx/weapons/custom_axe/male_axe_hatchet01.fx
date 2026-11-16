#############################################################
## Costume Anim Entity - Axe - 03
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		#Anim	CustomWeapon_Axe_03
		ChildFX	:FX_Hatchet_01.fx
	End
End

#############################################################

End