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
		Bhvr	WEAPONS\Custom_Claws\Huge_WepRScale.bhvr
		Anim	CustomWeapon_ClawsRight_FlatBlade01
	End
End

#############################################################

End