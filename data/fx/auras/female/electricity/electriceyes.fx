#############################################################
## ElectricEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

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
		ENAME	EyeNodeLOffset
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.35 0.31 0.30
		End
		part	:EyeArcs01.part
		POther	EyeNodeL
	End

	Event
		ENAME	EyeNodeROffset
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.35 0.31 0.30
		End
		part	:EyeArcs02.part
		POther	EyeNodeR
	End

	Event
		ENAME	EyeNodeL
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.0 -0.1
		End
		part	:EyeGlow01.part
		part	:EyeGlow02.part
	End

	Event
		ENAME	EyeNodeR
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.0 0.0 -0.1
		End
		part	:EyeGlow01.part
		part	:EyeGlow02.part
	End
End

#############################################################

End