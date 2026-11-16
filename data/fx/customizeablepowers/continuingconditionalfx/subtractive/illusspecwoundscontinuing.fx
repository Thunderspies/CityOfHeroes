#############################################################
## IllusSpecWoundsContinuing.fx
#############################################################

FxInfo
ClampMaxScale 2.0 2.0 2.0

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	Neck
		Sound glow6_loop 60 60 .22
	End

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	Chest
		Sound darkfear1 60 60 .3
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	WepR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	WepL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	Ulegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	LlegL
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	Llegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	FootL
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	LlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:IllusSWSkulls01.part
		POther	FootR
	End
End


#############################################################

End