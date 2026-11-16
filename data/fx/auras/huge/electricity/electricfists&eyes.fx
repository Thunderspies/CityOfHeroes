#############################################################
## ElectricFists.fx
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
			PositionOffset		0.0 -0.05 -0.1
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
			PositionOffset		0.0 -0.05 -0.1
		End
		part	:EyeGlow01.part
		part	:EyeGlow02.part
	End
End

#############################################################

Condition
	On	Cycle
	Time	60
	Chance	0.50

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 10
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:FistGlow01.part
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 10
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 10
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:FistGlow01.part
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 10
	End
End

#############################################################

Condition
	On	Cycle
	Time	58
	Chance	0.50

	Event
		ENAME	RightLarmGlowA
		Type	Local
		At	HandR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:FistGlow01.part
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	54
	Chance	.50

	Event
		ENAME	LeftLarmGlowA
		Type	Local
		At	HandL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:FistGlow01.part
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 10
	End
End

#############################################################

End