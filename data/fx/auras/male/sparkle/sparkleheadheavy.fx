#############################################################
## SparkleHeadHeavy.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:HeadSparkle02.part
		POther	Neck
	End
End

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:EyeSparkle02.part
		part	:EyeSparkle03.part
		part	:EyeGlow02.part
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:EyeSparkle02.part
		part	:EyeSparkle03.part
		part	:EyeGlow02.part
	End
End

#############################################################

End