#############################################################
## Blood Widow "Elude" FX
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound Fast6 80 80 .7
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	POWERS\SuperReflexes\SREludeBodyStreaks01.part
		part1	POWERS\SuperReflexes\SREludeShimmer01.part
		POther	Neck
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	POWERS\SuperReflexes\SREludeBodyStreaks01.part
		part1	POWERS\SuperReflexes\SREludeShimmer01.part
		POther	Hair
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	POWERS\SuperReflexes\SREludeBodyStreaks01.part
		part1	POWERS\SuperReflexes\SREludeShimmer01.part
		POther	LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	POWERS\SuperReflexes\SREludeBodyStreaks01.part
		part1	POWERS\SuperReflexes\SREludeShimmer01.part
		POther	LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	POWERS\SuperReflexes\SREludeBodyStreaks01.part
		part1	POWERS\SuperReflexes\SREludeShimmer01.part
		POther	Ulegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	POWERS\SuperReflexes\SREludeBodyStreaks01.part
		part1	POWERS\SuperReflexes\SREludeShimmer01.part
		POther	UlegR
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
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	Neck
		Lifespan 40
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	Chest
		Lifespan 40
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	LarmR
		Lifespan 40
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	LarmR
		Lifespan 40
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	LarmL
		Lifespan 40
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	LarmL
		Lifespan 40
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	Ulegl
		Lifespan 40
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	Llegl
		Lifespan 40
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	UlegR
		Lifespan 40
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\SuperReflexes\SRBodyGlowLines02.part
		POther	LlegR
		Lifespan 40
	End
End

#############################################################

End