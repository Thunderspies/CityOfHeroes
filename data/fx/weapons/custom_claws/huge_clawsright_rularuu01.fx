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
		Bhvr	WEAPONS\Custom_Claws\Huge_WepRScale.bhvr
		BhvrOverride
			PositionOffset		-0.05 0.03 0.025
			Scale			1.35 1.35 1.35
		End
		Anim	CustomWeapon_ClawsRight_Rularuu01
	End
End

#############################################################

End