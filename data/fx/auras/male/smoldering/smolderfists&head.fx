#############################################################
## SmolderFists.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## FISTS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandR
		part1	:BodyStreaks02small.part
	End

	Event
		Type	Local
		At	HandL
		part1	:BodyStreaks02small.part
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part1	:BodyStreaks01.part
		part	:BodyStreaks02.part
		part1	:BodySmoke01.part
		POther	LarmR
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part1	:BodyStreaks01.part
		part	:BodyStreaks02.part
		part1	:BodySmoke01.part
		POther	LarmL
	End
End

## HAIR ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part	:BodyStreaks01.part
		part	:HairStreaks01.part
		part	:BodySmoke01.part
		POther	Hair
	End

	Event
		Type	Local
		At	Eyes
		part	:BodyStreaks01.part
		part	:HairStreaks01.part
		part	:BodySmoke01.part
		POther	Hair
	End
End

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	EyeNodeL
		Type	Local
		At	Head
		Bhvr	:EyeNodeL.bhvr
		part	:EyeGlow01.part
	End

	Event
		ENAME	EyeNodeR
		Type	Local
		At	Head
		Bhvr	:EyeNodeR.bhvr
		part	:EyeGlow01.part
	End
End

#############################################################

End