#############################################################
## ElectricBody.fx
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
	Time	22
	Chance	.30

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	20
	Chance	.30

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	34
	Chance	.25

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet Chest
		POther  Chest
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	31
	Chance	.25

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
End

Condition
	On	Cycle
	Time	36
	Chance	.25

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		Bhvr	Behaviors\TelportationFade.bhvr
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
	Time	40
	Chance	.25

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
End

Condition
	On	Cycle
	Time	29
	Chance	.25

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	37
	Chance	.25

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	28
	Chance	.25

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	32
	Chance	.25

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	30
	Chance	.25

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 10
	End
End

#############################################################

End