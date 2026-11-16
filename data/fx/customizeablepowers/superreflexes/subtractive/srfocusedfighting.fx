#########################################################
##	template

FxInfo
#Lifespan 300


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Fast2 70 70 .7
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
		part1	:SRBodyGlowLines02.part
		POther Neck
		Lifespan 30

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRBodyGlowLines02.part
		POther Chest
		Lifespan 30

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Lifespan 30

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Lifespan 30

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Lifespan 30

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Lifespan 30

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:SRBodyGlowLines02.part
		POther Ulegl
		Lifespan 30

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:SRBodyGlowLines02.part
		POther Llegl
		Lifespan 30

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:SRBodyGlowLines02.part
		POther UlegR
		Lifespan 30

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:SRBodyGlowLines02.part
		POther LlegR
		Lifespan 30

	End


End

Condition
	On	Cycle
	Time	120


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRFFHeadCircles01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 30

	End


End


End