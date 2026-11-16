#############################################################
## SparkleHandsHeavy.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

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

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	WepR
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle02.part
		POther	WepL
	End
End

#############################################################

End