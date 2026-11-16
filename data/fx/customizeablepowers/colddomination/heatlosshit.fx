#############################################################
## HeatLossHit.fx
#############################################################

FxInfo
Lifespan 60

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound icepunchhit1 80 80 0.88
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		Lifespan 80
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	Neck
		Lifespan 80
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	Chest
		Lifespan 80
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	LarmR
		Lifespan 80
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	LarmL
		Lifespan 80
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	Ulegl
		Lifespan 80
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	Llegl
		Lifespan 80
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	UlegR
		Lifespan 80
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther	LlegR
		Lifespan 80
	End
End

#############################################################

End