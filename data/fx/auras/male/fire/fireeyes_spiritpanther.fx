#############################################################
## FireEyes.fx
#############################################################

FxInfo
Flags DontSuppress InheritAlpha ##- Don't use inheret alpha on Aura FX

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Head
		BhvrOverride
//			Scale		3 3 3
			PositionOffset	0 -0.05 0.3
			TintGeom	1
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
//		part	:EyeGlow01.part
//		part	:EyeGlow02.part
		part	:FireEyesLeft02.part
		part	:FireEyesLeft02.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
//		part	:EyeGlow01.part
//		part	:EyeGlow02.part
		part	:FireEyesRight02.part
		part	:FireEyesRight02.part
	End
End

#############################################################

End