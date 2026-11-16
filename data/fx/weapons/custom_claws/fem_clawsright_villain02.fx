#############################################################
## Costume Anim Entity - Right Claws - Base
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
		Bhvr	WEAPONS\Custom_Claws\Female_WepRScale.bhvr
		Anim	CustomWeapon_ClawsRight_Villain02
	End
End

#############################################################

End