#############################################################
## SparkleHandsLight.fx
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
End

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

## EYES ###########################################################

Condition
	On	Cycle
	Time	25
	Chance	0.3

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		part	:EyeSparkle01.part
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	30
	Chance	0.3

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		part	:EyeSparkle01.part
		Lifespan 5
	End
End

#############################################################

End