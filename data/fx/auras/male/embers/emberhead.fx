#############################################################
## Header
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

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


Condition
	On	Time
	Time	0

	Event
		ENAME	HeadNode
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
	End

	Event
		Type	Local
		At	HeadNode
		part	:HeadEmbers01.part
		Pmagnet Head
	End

End

End