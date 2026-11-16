#############################################################
## StarGlowHead.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	:EyeNodeL.bhvr
		part	:EyeStarGlow01.part
		part	:EyeStarGlow02.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:EyeNodeR.bhvr
		part	:EyeStarGlow01.part
		part	:EyeStarGlow02.part
	End
End

## HAIR ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		part	:HeadStarGlow01.part
		part	:HeadStarGlow02.part
		Pmagnet Head
	End
End

#############################################################

End