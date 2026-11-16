#############################################################
## Costume Anim Entity - Axe - 04
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
		#Anim	CustomWeapon_Axe_04
		ChildFX	:FX_RiktiAxe_01.fx
	End
End

#############################################################

End