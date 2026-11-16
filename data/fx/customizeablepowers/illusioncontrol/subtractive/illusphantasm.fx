#############################################################
## IllusPhantasm.fx
#############################################################

FxInfo
LifeSpan 60

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	Origin
		Sound invisible1 100 100 .6
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	20


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:IllusPhantBodyGlow01.part
		POther Neck
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:IllusPhantBodyGlow01.part
		POther Chest
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:IllusPhantBodyGlow01.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:IllusPhantBodyGlow01.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:IllusPhantBodyGlow01.part
		POther LarmL
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:IllusPhantBodyGlow01.part
		POther LarmL
		Lifespan 0

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:IllusPhantBodyGlow01.part
		POther Ulegl
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:IllusPhantBodyGlow01.part
		POther Llegl
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:IllusPhantBodyGlow01.part
		POther UlegR
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:IllusPhantBodyGlow01.part
		POther LlegR
		Lifespan 0

	End


End


End