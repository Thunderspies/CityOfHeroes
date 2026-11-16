#############################################################
## SparkleBodyHeavy.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Head
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	Neck
	End

	Event
		Type	Local
		At	Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther  Chest
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther  UArmR
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	UArmR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	UArmL
	End


	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	UArmL
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	LArmR
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	LArmL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	WepR
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	WepL
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	ULegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	LLegR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	LLegL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	FootR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	FootL
	End
End

#############################################################

End