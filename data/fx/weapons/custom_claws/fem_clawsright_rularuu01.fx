#############################################################
## Costume Anim Entity - Right Claws - 05
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmR
		Bhvr	WEAPONS\Custom_Claws\Female_WepRScale.bhvr
		BhvrOverride
			PositionOffset		0.1 0.0 0.05
		End
		Anim	CustomWeapon_ClawsRight_Rularuu01
	End
End

#############################################################

End