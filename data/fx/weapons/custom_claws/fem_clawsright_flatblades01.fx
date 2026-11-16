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
		BhvrOverride
			Scale	0.6875 0.6875 0.6875
		End
		Anim	CustomWeapon_ClawsRight_FlatBlade01
	End
End

#############################################################

End