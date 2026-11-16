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
			PositionOffset	0.0 0.0 -0.1
		End
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		part	:FireEyesLeft01.part
		part	:FireEyesLeft01.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -0.1
		End
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		part	:FireEyesRight01.part
		part	:FireEyesRight01.part
	End
End

#############################################################

End