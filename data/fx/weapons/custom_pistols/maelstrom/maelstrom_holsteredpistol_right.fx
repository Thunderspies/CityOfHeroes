#############################################################
## Test.fx
#############################################################

FxInfo

Flags InheritAlpha

## HOLSTERED ###########################################################

Condition
	On 	Time
	Time	0

	Event
		EName	HolsteredPisol_Right
		Type	Local
		At	ULegR
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HolsteredPistols_Fade.bhvr
		BhvrOverride
			PositionOffset		0.375 -0.0 -0.35
			PYRRotate		5 0 85
		End
		Anim	CustomWeapon_PistolLeft_Maelstrom_Holstered
	End
End

#############################################################

End