#############################################################
## SparkleBodyLight.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	7
	Chance	0.30

	Event
		ENAME	Head
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	Neck
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	8
	Chance	0.30

	Event
		Type	Local
		At	Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther  Chest
		Lifespan 5
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther  UArmL
		Lifespan 5
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther  UArmR
		Lifespan 5
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	UArmR
		Lifespan 5
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	UArmL
		Lifespan 5
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	UArmL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	9
	Chance	0.30

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	ULegL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	LArmR
		Lifespan 5
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	WepR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	11
	Chance	0.30

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	LArmL
		Lifespan 5
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	WepL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	12
	Chance	0.30

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	LLegR
		Lifespan 5
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	LLegL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	13
	Chance	0.30

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	FootR
		Lifespan 5
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther	FootL
		Lifespan 5
	End
End

#############################################################

End