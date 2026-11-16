#############################################################
## FireEyes.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.15
		End
		Part CustomizeablePowers\DarknessControl\Eyes_Glow_Round_Small.part
		Part CustomizeablePowers\DarknessControl\Eyes_Glow_Slanted_Small.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.15
		End
		Part CustomizeablePowers\DarknessControl\Eyes_Glow_Round_Small.part
		Part CustomizeablePowers\DarknessControl\Eyes_Glow_Slanted_Inverted_Small.part
	End
End

#############################################################

End