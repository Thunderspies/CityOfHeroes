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


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound icepunchhit1 80 80 .88
	End
End





#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	5


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		#POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End


#	Event
#		ENAME	RightUarmGlow
#		Type	Local
#		At	SpadR
#		part	:ColdCirclesDark.part
#		part	:ColdCircles02.part
#		POther LarmR
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#
#	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End


#	Event
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	SpadL
#		part	:ColdCirclesDark.part
#		part	:ColdCircles02.part
#		POther LarmL
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#
#	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:ColdCirclesDark.part
		part	:ColdCircles02.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80

	End


End


End