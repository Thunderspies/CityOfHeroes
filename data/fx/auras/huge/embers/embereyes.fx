#############################################################
## Header
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	EyeNodeL
		Type	Local
		At	Head
		part	:EyeGlow01.part
		part	:EyeEmbers01.part
		part	:HeadEmbersTrail01.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event
		ENAME	EyeNodeR
		Type	Local
		At	Head
		part	:EyeGlow01.part
		part	:EyeEmbers01.part
		part	:HeadEmbersTrail01.part
		Bhvr	:EyeNodeR.bhvr
	End

End

End