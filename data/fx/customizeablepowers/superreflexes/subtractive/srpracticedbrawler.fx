#############################################################
## SRPracticedBrawler.fx
#############################################################

FxInfo
Lifespan 15

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Fast3 80 80 .8
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	15


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:SRBodyGlowLines02.part
		POther Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRBodyGlowLines02.part
		POther Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:SRBodyGlowLines02.part
		POther Ulegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:SRBodyGlowLines02.part
		POther Llegl
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:SRBodyGlowLines02.part
		POther UlegR
		Lifespan 15

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:SRBodyGlowLines02.part
		POther LlegR
		Lifespan 15

	End


End



End