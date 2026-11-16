#############################################################
## SRPracticedBrawlerContinuing.fx
#############################################################

FxInfo


#########################################################
#################### BODY GLOW 2 ########################
#########################################################

Condition
	On	Time
	Time	15

	Event
		EName	2
		Type	Local
		At	Chest
		Sound fast7_loop 60 60 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End
End


Condition
	On	Cycle
	Time	60


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:SRBodyGlowLines02.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:SRBodyGlowLines02.part
		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:SRBodyGlowLines02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:SRBodyGlowLines02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:SRBodyGlowLines02.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:SRBodyGlowLines02.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:SRBodyGlowLines02.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:SRBodyGlowLines02.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


End


End