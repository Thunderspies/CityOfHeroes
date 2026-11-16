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
		EName	HolsteredPisol_Left
		Type	Local
		At	ULegL
		Bhvr	WEAPONS\Custom_Pistols\Maelstrom\Maelstrom_HolsteredPistols_Fade.bhvr
		BhvrOverride
			PositionOffset		-0.4 -0.0 -0.35
			PYRRotate		5 0 95
		End
		Anim	CustomWeapon_PistolLeft_Maelstrom_Holstered
	End
End

#############################################################

End