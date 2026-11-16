#############################################################
## SparkleHandsLight.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	5
	Chance	0.30

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther  LarmR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	9
	Chance	0.30

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySparkle01.part
		POther  LarmL
		Lifespan 5
	End
End

#############################################################

End