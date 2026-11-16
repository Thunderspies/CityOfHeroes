#############################################################
## SparkleEyesLight.fx
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
End

Condition
	On	Cycle
	Time	25
	Chance	0.3

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		part	:EyeSparkle01.part
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	30
	Chance	0.3

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		part	:EyeSparkle01.part
		Lifespan 5
	End
End

#############################################################

End