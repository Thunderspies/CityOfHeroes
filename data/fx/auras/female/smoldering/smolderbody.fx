#############################################################
## SmolderBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:BodySmoke01.part
		POther Neck

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	UarmR
		part1	:BodyStreaks02.part
		POther Neck

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	UarmL
		part1	:BodyStreaks02.part
		POther Neck

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:BodyStreaks01.part
		part	:BodyStreaks02.part
		part	:BodySmoke01.part
		POther Hips

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part1	:BodySmoke01.part
		POther LarmR

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part1	:BodyStreaks01.part
		part	:BodyStreaks02.part
		part1	:BodySmoke01.part
		POther LarmR

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part1	:BodySmoke01.part
		POther LarmL

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part1	:BodyStreaks01.part
		part	:BodyStreaks02.part
		part1	:BodySmoke01.part
		POther LarmL

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:BodySmoke01.part
		POther Ulegl

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:BodyStreaks01.part
		part	:BodyStreaks02.part
		part1	:BodySmoke01.part
		POther Llegl

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:BodySmoke01.part
		POther UlegR

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:BodyStreaks01.part
		part	:BodyStreaks02.part
		part1	:BodySmoke01.part
		POther LlegR

	End


End



End