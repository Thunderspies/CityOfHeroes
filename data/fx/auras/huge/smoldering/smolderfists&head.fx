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
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		ENAME	EyeNodeL
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		part	:EyeGlow01.part
	End

	Event
		ENAME	EyeNodeR
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.1
		End
		part	:EyeGlow01.part
	End
End

#############################################################

End