#############################################################
## Header
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	25
	Chance	.3

	Event
		ENAME	EyeNodeL
		Type	Local
		At	Head
		part	:EyeSparkle01.part
		Bhvr	:EyeNodeL.bhvr
		Lifespan 5
	End

End

Condition
	On	Cycle
	Time	30
	Chance	.3

	Event
		ENAME	EyeNodeR
		Type	Local
		At	Head
		part	:EyeSparkle01.part
		Bhvr	:EyeNodeR.bhvr
		Lifespan 5
	End

End

End