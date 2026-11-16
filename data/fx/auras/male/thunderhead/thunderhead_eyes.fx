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
		Geom	CircleOfThorns
	End

	Event
		EName	EyeAnchorL
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -0.05
		End
		Part	:Eyes_Glow.part
	End

	Event
		EName	EyeAnchorR
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -0.05
		End
		Part	:Eyes_Glow.part
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.25

	Event
		ENAME	EyeNodeL
		Type	Local
		At	EyeAnchorL
		part	:Eyes_Glow2.part
		Lifespan 3
	End

	Event
		ENAME	EyeAnchorL
		Type	Local
		At	Head
		Bhvr	:EyeNodeOffsetL.bhvr
		part	:Eyes_Arcs.part
		Lifespan	4
		POther	EyeNodeL

	End


End

Condition
	On	Cycle
	Time	30
	Chance	0.5

	Event
		ENAME	EyeNodeR
		Type	Local
		At	EyeAnchorR
		part	:Eyes_Glow2.part
		Lifespan 3
	End

	Event
		ENAME	EyeAnchorR
		Type	Local
		At	Head
		Bhvr	:EyeNodeOffsetR.bhvr
		part	:Eyes_Arcs.part
		Lifespan	4
		POther	EyeNodeR
	End
End

#############################################################

End