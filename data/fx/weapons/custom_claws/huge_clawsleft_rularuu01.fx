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
		At	LArmL
		Bhvr	WEAPONS\Custom_Claws\Huge_LArmLScaleAndFlip.bhvr
		BhvrOverride
			PositionOffset		0.1 0.03 0.025
			Scale			1.35 1.35 1.35
		End
		Anim	CustomWeapon_ClawsRight_Rularuu01
	End
End

#############################################################

End