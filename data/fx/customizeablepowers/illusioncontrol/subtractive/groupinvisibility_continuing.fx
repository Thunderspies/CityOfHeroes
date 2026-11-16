#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	Neck
		Sound Invisible1 80 80 .46
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	Chest
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	Ulegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	UlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IllusionControl\IllusBodyGlow01.part
		POther	LlegR
	End
End

#############################################################

End