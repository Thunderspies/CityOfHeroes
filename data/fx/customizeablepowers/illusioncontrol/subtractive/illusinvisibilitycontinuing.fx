#############################################################
## IllusInvisibilityContinuing.fx
#############################################################

FxInfo

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	Neck
		Sound Invisible1 80 80 .46
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	Chest
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	Ulegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	UlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusBodyGlow01.part
		POther	LlegR
	End
End

#############################################################

End