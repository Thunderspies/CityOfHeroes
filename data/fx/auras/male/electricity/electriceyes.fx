#############################################################
## ElectricEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	EyeNodeLOffset
		Type	Local
		At	Head
		Bhvr	:EyeNodeOffsetL.bhvr
		part	:EyeArcs01.part
		POther	EyeNodeL
	End

	Event
		ENAME	EyeNodeROffset
		Type	Local
		At	Head
		Bhvr	:EyeNodeOffsetR.bhvr
		part	:EyeArcs02.part
		POther	EyeNodeR
	End

	Event
		ENAME	EyeNodeL
		Type	Local
		At	Head
		Bhvr	:EyeNodeL.bhvr
		part	:EyeGlow01.part
		part	:EyeGlow02.part
	End

	Event
		ENAME	EyeNodeR
		Type	Local
		At	Head
		Bhvr	:EyeNodeR.bhvr
		part	:EyeGlow01.part
		part	:EyeGlow02.part
	End
End

#############################################################

End