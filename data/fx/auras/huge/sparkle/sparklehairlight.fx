#############################################################
## SparkleHeadLight.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## HAIR ###########################################################

Condition
	On	Cycle
	Time	10
	Chance	0.30

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:HeadSparkle01.part
		POther	Neck
		Lifespan 5
	End
End

#############################################################

End